#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <inttypes.h>

// função main que trata o recebimento: ./client 127.0.0.1 90100 OU ./client 127.0.0.1 90200
// o cliente recebe o endereço IP e a porta do servidor
int main(int argc, char **argv)
{
    // printf("Arquivo client.c!\n");

    char enderecoIP[20];
    strcpy(enderecoIP, argv[1]);
    int portaDoServidor = atoi(argv[2]);
    // printf("Endereço IP do servidor: %s\n", enderecoIP);
    // printf("Porta do servidor: %d\n", portaDoServidor);

    char killOuShow[5] = "";
    char showFuncionalidade[20] = "";
    char mensagemDoClienteParaServidor[50];
    char mensagemDoServidorParaCliente[50];
    char* idDoCliente;
    int idDoClienteInt;

    // criando o socket com o portocolo de transporte TCP e protocolo de rede IPv4
    // FUNÇÃO SOCKET()
    int socketCliente;
    socketCliente = socket(AF_INET, SOCK_STREAM, 0);
    if (socketCliente == -1) {
        printf("--- Erro ao criar o socket!\n");
    }
    else {
        printf("--- Socket criado com sucesso!\n");
    }

    // Abertura de comunicação de Cliente com Servidor
    // 1. Um cliente Ci solicita ao servidor Mi a abertura de comunicação a fim de obter seu identificador na rede.
    // FUNÇÃO CONNECT()
    struct sockaddr_in enderecoServidor;
    enderecoServidor.sin_family = AF_INET;
    enderecoServidor.sin_port = htons(portaDoServidor);
    enderecoServidor.sin_addr.s_addr = inet_addr(enderecoIP);
    int conectou = connect(socketCliente, (struct sockaddr *)(&enderecoServidor), sizeof(enderecoServidor));
    if (conectou == -1) 
    {
        printf("--- Erro CONNECT com o servidor!\n");

        // print ERROR 01
        printf("Client limit exceeded\n");
        return 0;
    }
    else 
    {
        printf("--- CONNECT com sucesso!\n");

        // Se conseguiu se conectar, envia (send(REQ_ADD)), servidor recebe a requisição (recv(REQ_ADD)) e responde com o ID do cliente 
        // (send(RES_ADD(Ci)), cliente recebe a resposta (recv(RES_ADD(Ci)) e imprime a mensagem "New ID: Ci"

        // FUNÇÃO SEND()
        send(socketCliente, "REQ_ADD", strlen("REQ_ADD"), 0);
        printf("--- REQ_ADD enviado\n");

        // FUNÇÃO RECV()
        recv(socketCliente, mensagemDoServidorParaCliente, 50, 0);
        // printf("Mensagem recebida no connect: %s\n", mensagemDoServidorParaCliente);
        idDoCliente = strtok(mensagemDoServidorParaCliente, "(");
        idDoCliente = strtok(NULL, ")");
        printf("New ID: %s\n", idDoCliente);
        idDoClienteInt = atoi(idDoCliente);
    }

    // MENSAGENS QUE O CLIENTE PODE RECEBER DO TERMINAL:
    // kill
    // show localmaxsensor
    // show externalmaxsensor
    // show localpotency
    // show externalpotency
    // show globalmaxsensor
    // show globalmaxnetwork

    // enquanto não recebe o comando "kill", o cliente fica recebendo comandos do usuário e enviando para o servidor
    while(1)
    {
        printf("--- Entrou no WHILE\n");

        // zera as variáveis que vão ser lidas antes de cada leitura
        memset(killOuShow, 0, 5);
        memset(showFuncionalidade, 0, 20);
        memset(mensagemDoClienteParaServidor, 0, 50);
        memset(mensagemDoServidorParaCliente, 0, 50);

        scanf("%s", killOuShow);

        // se não for "kill" nem "show", printa Comando inválido e volta a pedir um comando
        if (strcmp(killOuShow, "kill") != 0 && strcmp(killOuShow, "show") != 0) 
        {
            printf("Comando inválido\n");
            continue;
        }
        // se for "show", lê a funcionalidade
        if (strcmp(killOuShow, "show") == 0) 
        {
            scanf("%s", showFuncionalidade);

            // printf("Variável showFuncionalidade: %s\n", showFuncionalidade);

            // se for "localmaxsensor", envia a mensagem "REQ_LS" para o servidor
            if (strcmp(showFuncionalidade, "localmaxsensor") == 0) 
            {
                strcpy(mensagemDoClienteParaServidor, "REQ_LS");
            }
            // se for "externalmaxsensor", envia a mensagem "REQ_ES" para o servidor
            else if (strcmp(showFuncionalidade, "externalmaxsensor") == 0) 
            {
                strcpy(mensagemDoClienteParaServidor, "REQ_ES");
            }
            // se for "localpotency", envia a mensagem "REQ_LP" para o servidor
            else if (strcmp(showFuncionalidade, "localpotency") == 0) 
            {
                strcpy(mensagemDoClienteParaServidor, "REQ_LP");
            }
            // se for "externalpotency", envia a mensagem "REQ_EP" para o servidor
            else if (strcmp(showFuncionalidade, "externalpotency") == 0) 
            {
                strcpy(mensagemDoClienteParaServidor, "REQ_EP");
            }
            // se for "globalmaxsensor", envia a mensagem "REQ_MS" para o servidor
            else if (strcmp(showFuncionalidade, "globalmaxsensor") == 0) 
            {
                strcpy(mensagemDoClienteParaServidor, "REQ_MS");
            }
            // se for "globalmaxnetwork", envia a mensagem "REQ_MN" para o servidor
            else if (strcmp(showFuncionalidade, "globalmaxnetwork") == 0) 
            {
                strcpy(mensagemDoClienteParaServidor, "REQ_MN");
            }
            // se receber um comando inválido, printa e volta a pedir um comando
            else
            {
                printf("Comando inválido\n");
                continue;
            }

        }
        // se for "kill", envia a mensagem "REQ_DC(idDoCliente)" para o servidor
        else if (strcmp(killOuShow, "kill") == 0) 
        {
            // coloca na variável mensagemDoClienteParaServidor a mensagem "REQ_DC(idDoCliente)"
            snprintf(mensagemDoClienteParaServidor, 50, "REQ_DC(%d)", idDoClienteInt);
            
            // printf("Mensagem do cliente para o servidor: %s\n", mensagemDoClienteParaServidor);

            // FUNÇÃO SEND()
            send(socketCliente, mensagemDoClienteParaServidor, strlen(mensagemDoClienteParaServidor), 0);
            printf("--- Send enviado: %s\n", mensagemDoClienteParaServidor);
        
            // FUNÇÃO RECV()
            recv(socketCliente, mensagemDoServidorParaCliente, 50, 0);

            // se recebeu ERROR(04) imprime "Client not found"
            if (strcmp(mensagemDoServidorParaCliente, "ERROR(04)") == 0) 
            {
                printf("Client not found\n");
            }
            // se recebeu OK(01), imprime "Successful disconnect" fecha a conexão e encerra a execução
            else if (strcmp(mensagemDoServidorParaCliente, "OK(01)") == 0) 
            {
                printf("Successful disconnect\n");
                // fecha a conexão
                close(socketCliente);
                // encerra a execução
                return 0;
            }

        }

        // FUNÇÃO SEND()
        send(socketCliente, mensagemDoClienteParaServidor, strlen(mensagemDoClienteParaServidor), 0);
        printf("--- Send enviado: %s\n", mensagemDoClienteParaServidor);

        // inicializa um novo buffer com 0 pois agora ele vai receber a mensagem do servidor
        memset(mensagemDoServidorParaCliente, 0, 50);
        // FUNÇÃO RECV()
        recv(socketCliente, mensagemDoServidorParaCliente, 50, 0);
        printf("--- MENSAGEM RECEBIDA: %s\n", mensagemDoServidorParaCliente);

        // separa a mensagem recebida em comando e mensagemDentroDoComando
        // Ex: recebe RES_LS(local 0 sensor 0: 150 (307 49)), coloca em comando = RES_LS e mensagemDentroDoComando = local 0 sensor 0: 150 (307 49)
        // e imprime a mensagemDentroDoComando
        char* comando;
        char* mensagemDentroDoComando;
        comando = strtok(mensagemDoServidorParaCliente, "(");
        mensagemDentroDoComando = strtok(NULL, ")");
        // adiciona um ) no final da mensagemDentroDoComando, se o comando for REQ_LP
        if (strcmp(comando, "RES_LS") == 0) 
        {
            strcat(mensagemDentroDoComando, ")");
        }
        printf("%s\n", mensagemDentroDoComando);

    }
    
    // FUNÇÃO CLOSE()
    close(socketCliente);

    return 0;
}










































































// #define BUFFER_SIZE 500

// // função main que trata o recebimento: ./client v4 127.0.0.1 90900 OU ./client v6 ::1 90900
// int main(int argc, char **argv)
// {
//     // printf("Arquivo client.c!\n");
//     char versaoDoProtocolo[5];
//     strcpy(versaoDoProtocolo, argv[1]);
//     // coloca na variável enderecoIP o endereço IP do servidor recebido junto com a execução do programa. Exemplo: 127.0.0.1
//     char enderecoIP[20];
//     strcpy(enderecoIP, argv[2]);
//     // coloca na variável portaDoServidor a porta do servidor recebida junto com a execução do programa. Exemplo: 90900
//     int portaDoServidor = atoi(argv[3]);

//     char funcionalidade[10] = "";
//     char parametroOuArquivo[10] = "";
//     char nomeDoArquivo[20] = "";
//     char sensorId[5] = "";
//     char corrente[5] = "";
//     char tensao[5] = "";
//     char eficienciaEnergetica[5] = "ZERO";
//     char acao[10];
//     char mensagemDoClienteParaServidor[50];
//     char mensagemDoServidorParaCliente[50];

//     // criando o socket com o portocolo de transporte TCP e protocolo de rede IPv4
//     // FUNÇÃO SOCKET()
//     int socketCliente;
//     // se a versão for v4, cria um socket IPv4
//     if (strcmp(versaoDoProtocolo, "v4") == 0)
//     {
//         socketCliente = socket(AF_INET, SOCK_STREAM, 0);
//     }
//     // se a versão for v6, cria um socket IPv6
//     // else if (strcmp(versaoDoProtocolo, "v6") == 0)
//     else
//     {
//         socketCliente = socket(AF_INET6, SOCK_STREAM, 0);
//     }
//     // socketCliente = socket(AF_INET, SOCK_STREAM, 0);

//     // FUNÇÃO CONNECT()
//     struct sockaddr_in enderecoServidor;
//     enderecoServidor.sin_family = AF_INET;
//     enderecoServidor.sin_port = htons(portaDoServidor);
//     enderecoServidor.sin_addr.s_addr = INADDR_ANY;
//     connect(socketCliente, (struct sockaddr *)(&enderecoServidor), sizeof(enderecoServidor));

    // enquanto não recebe o comando "kill", o cliente fica recebendo comandos do usuário e enviando para o servidor
//     while(1)
//     {
//         // zera as variáveis que vão ser lidas antes de cada leitura
//         memset(funcionalidade, 0, 10);
//         memset(parametroOuArquivo, 0, 10);
//         memset(nomeDoArquivo, 0, 20);
//         memset(sensorId, 0, 5);
//         memset(corrente, 0, 5);
//         memset(tensao, 0, 5);
//         memset(eficienciaEnergetica, 0, 5);
//         memset(acao, 0, 10);
//         memset(mensagemDoClienteParaServidor, 0, 50);

//         scanf("%s", funcionalidade);
//         // se a funcionalidade for "kill", envia a mensagem "kill" para o servidor e encerra o programa
//         if (strcmp(funcionalidade, "kill") == 0) 
//         {
//             strcpy(mensagemDoClienteParaServidor, "kill");
//             send(socketCliente, mensagemDoClienteParaServidor, strlen(mensagemDoClienteParaServidor), 0);
//             break;
//             return 0;
//         }

//         scanf("%s", parametroOuArquivo);
//         // se parametroOuArquivo == "param", então o usuário vai digitar os valores de sensorId, corrente, tensao e eficienciaEnergetica
//         if (strcmp(parametroOuArquivo, "param") == 0) 
//         {
//             scanf("%s", sensorId);
//             if(atoi(sensorId) < 0)
//             {
//                 printf("Comando inválido!\n");
//                 continue;
//             }
//             scanf("%s", corrente);
//             if(atoi(corrente) < 0 || atoi(corrente) > 10)
//             {
//                 printf("Comando inválido!\n");
//                 continue;
//             }
//             scanf("%s", tensao);
//             if(atoi(tensao) < 0 || atoi(tensao) > 150)
//             {
//                 printf("Comando inválido!\n");
//                 continue;
//             }
//             scanf("%s", eficienciaEnergetica);
//             if(atoi(eficienciaEnergetica) < 0 || atoi(eficienciaEnergetica) > 100)
//             {
//                 printf("Comando inválido!\n");
//                 continue;
//             }
//         }
//         // se parametroOuArquivo == "file", então o usuário vai digitar o nome do arquivo que contém os valores de sensorId, corrente, tensao e eficienciaEnergetica
//         else if (strcmp(parametroOuArquivo, "file") == 0) 
//         {
//             scanf("%s", nomeDoArquivo);
//             // lê do arquivo os valores de sensorId, corrente, tensao e eficienciaEnergetica
//             FILE *arquivo;
//             arquivo = fopen(nomeDoArquivo, "r");
//             if (arquivo == NULL) 
//             {
//                 printf("Erro ao abrir o arquivo!\n");
//                 return 0;
//             }
//             else
//             {
//                 fscanf(arquivo, "%s", sensorId);
//                 if (atoi(sensorId) < 0)
//                 {
//                     printf("Comando inválido!\n");
//                     continue;
//                 }
//                 fscanf(arquivo, "%s", corrente);
//                 if (atoi(corrente) < 0 || atoi(corrente) > 10)
//                 {
//                     printf("Comando inválido!\n");
//                     continue;
//                 }
//                 fscanf(arquivo, "%s", tensao);
//                 if (atoi(tensao) < 0 || atoi(tensao) > 150)
//                 {
//                     printf("Comando inválido!\n");
//                     continue;
//                 }
//                 fscanf(arquivo, "%s", eficienciaEnergetica);
//                 if (atoi(eficienciaEnergetica) < 0 || atoi(eficienciaEnergetica) > 100)
//                 {
//                     printf("Comando inválido!\n");
//                     continue;
//                 }
//             }
//         }

//         // se funcionalidade == "show" e parametroOuArquivo == "value", então o usuário vai digitar o sensorId
//         else if (strcmp(funcionalidade, "show") == 0 && strcmp(parametroOuArquivo, "value") == 0)
//         {
//             scanf("%s", sensorId);
//         }

//         // se funcionalidade == "install", o cliente envia a ação INS_REQ para o servidor
//         if (strcmp(funcionalidade, "install") == 0) 
//         {
//             // if pra verificar os parametros
//             strcpy(acao, "INS_REQ");
//             // concatena a variável acao, sensorId, corrente, tensao e eficienciaEnergetica em uma variável mensagemDoClienteParaServidor
//             strcpy(mensagemDoClienteParaServidor, acao);
//             strcat(mensagemDoClienteParaServidor, " ");
//             strcat(mensagemDoClienteParaServidor, sensorId);
//             strcat(mensagemDoClienteParaServidor, " ");
//             strcat(mensagemDoClienteParaServidor, corrente);
//             strcat(mensagemDoClienteParaServidor, " ");
//             strcat(mensagemDoClienteParaServidor, tensao);
//             strcat(mensagemDoClienteParaServidor, " ");
//             strcat(mensagemDoClienteParaServidor, eficienciaEnergetica);
//         }
//         // se funcionalidade == "change", o cliente envia a ação CH_REQ para o servidor
//         else if (strcmp(funcionalidade, "change") == 0) 
//         {
//             strcpy(acao, "CH_REQ");
//             // concatena a variável acao, sensorId, corrente, tensao e eficienciaEnergetica em uma variável mensagemDoClienteParaServidor
//             strcpy(mensagemDoClienteParaServidor, acao);
//             strcat(mensagemDoClienteParaServidor, " ");
//             strcat(mensagemDoClienteParaServidor, sensorId);
//             strcat(mensagemDoClienteParaServidor, " ");
//             strcat(mensagemDoClienteParaServidor, corrente);
//             strcat(mensagemDoClienteParaServidor, " ");
//             strcat(mensagemDoClienteParaServidor, tensao);
//             strcat(mensagemDoClienteParaServidor, " ");
//             strcat(mensagemDoClienteParaServidor, eficienciaEnergetica);
//         }
//         // se funcionalidade == "show" e parametroOuArquivo == "value", vai receber ex: "show value 2", e envia ao servidor a mensagem "SEN_REQ 2"
//         else if (strcmp(funcionalidade, "show") == 0 && strcmp(parametroOuArquivo, "value") == 0)
//         {
//             strcpy(acao, "SEN_REQ");
//             // concatena a variável acao, sensorId, corrente, tensao e eficienciaEnergetica em uma variável mensagemDoClienteParaServidor
//             strcpy(mensagemDoClienteParaServidor, acao);
//             strcat(mensagemDoClienteParaServidor, " ");
//             strcat(mensagemDoClienteParaServidor, sensorId);
//         }
//         // se funcionalidade == "show" e parametroOuArquivo == "values", vai receber ex: "show values", e envia ao servidor a mensagem "VAL_REQ"
//         else if (strcmp(funcionalidade, "show") == 0 && strcmp(parametroOuArquivo, "values") == 0)
//         {
//             strcpy(acao, "VAL_REQ");
//             strcpy(mensagemDoClienteParaServidor, acao);
//         }
//         // se funcionalidade == "remove", vai receber ex: "remove 2", e envia ao servidor a mensagem "REM_REQ 2"
//         else if (strcmp(funcionalidade, "remove") == 0)
//         {
//             strcpy(acao, "REM_REQ");
//             // concatena a variável acao, sensorId, corrente, tensao e eficienciaEnergetica em uma variável mensagemDoClienteParaServidor
//             strcpy(mensagemDoClienteParaServidor, acao);
//             strcat(mensagemDoClienteParaServidor, " ");
//             strcat(mensagemDoClienteParaServidor, parametroOuArquivo);
//         }
//         // se recebeu um comando inválido, encerra a conexão e sua execução
//         else
//         {
//             printf("Comando inválido!\n");
//             close(socketCliente);
//             return 0;
//         }

//         // FUNÇÃO SEND()
//         send(socketCliente, mensagemDoClienteParaServidor, strlen(mensagemDoClienteParaServidor), 0);

//         // inicializa um novo buffer com 0 pois agora ele vai receber a mensagem do servidor
//         memset(mensagemDoServidorParaCliente, 0, 50);
//         // FUNÇÃO RECV()
//         recv(socketCliente, mensagemDoServidorParaCliente, 50, 0);

//         printf("%s\n", mensagemDoServidorParaCliente);
//     }

//     // FUNÇÃO CLOSE()
//     close(socketCliente);

//     return 0;
// }