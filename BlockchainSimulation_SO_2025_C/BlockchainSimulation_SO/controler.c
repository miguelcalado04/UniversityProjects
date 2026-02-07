//Cláudio Catarino 2022224320
//Miguel Calado 2022228235
#include "funcs.h"

#define BUFFER 500
#define key1 1234
#define key2 4321

int shmid, shmid2, mqid,tam;
int val_aux;
transactions_Pool *trans_Pool;
blockchain_Ledger *ledger;
pid_t pid1, pid2, pid3, pid4, pid5;
int TRANSACTIONS_PER_BLOCK;
pthread_t thread_validatores_aux;
pid_t main_pid;
pthread_t input_thread;



int init_trans_sem() {
    sem_transactions = sem_open("/sem_transactions", O_CREAT, 0777, 1);
    if (sem_transactions == SEM_FAILED) {
        perror("sem_open failed");
        return -1;
    }
    //printf("sem_transactions initialized\n");
    return 0;
}

int destroy_trans_sem() {
    if (sem_transactions != NULL && sem_transactions != SEM_FAILED) {
        sem_close(sem_transactions); 
        sem_unlink("/sem_transactions");
    }
    //printf("sem_transactions destroyed\n");
    return 0;
}

int init_block_sem() {
    sem_blockchain = sem_open("/sem_blockchain", O_CREAT, 0777, 1);
    if (sem_blockchain == SEM_FAILED) {
        perror("sem_open failed");
        return -1;
    }
    //printf("sem_blockchain initialized\n");
    return 0;
}

int destroy_block_sem() {
    if (sem_blockchain != NULL && sem_blockchain != SEM_FAILED) {
        sem_close(sem_blockchain); 
        sem_unlink("/sem_blockchain");
    }
    //printf("sem_blockchain destroyed\n");
    return 0;
}

void clean() {
    
    if (getpid() != main_pid) return;
    
    //printf("\nSIGINT detected. Cleaning up...\n");
    logwrite( "CONTROLLER: SIGNAL SIGINT RECEIVED\n");
    logwrite("CONTROLLER: WAITING FOR LAST TASKS TO FINISH\n");

    
    pthread_cancel(thread_validatores_aux);

    if (trans_Pool) {
        shmdt(trans_Pool);
    }

    

    sem_wait(print_sem);
    waitpid(pid1, NULL, 0);
    sem_post(print_sem);
    sem_wait(print_sem);
    waitpid(pid2, NULL, 0);
    sem_post(print_sem);
    waitpid(pid3, NULL, 0);
     // Kill pid4 if running
    if (pid4 > 0) {
        sem_wait(print_sem);
        kill(pid4, SIGTERM);
        waitpid(pid4, NULL, 0);
        //printf("[INFO] Validator60 (pid4) terminated during cleanup.\n");
        sem_post(print_sem);
    }

    // Kill pid5 if running
    if (pid5 > 0) {
        sem_wait(print_sem);
        kill(pid5, SIGTERM);
        waitpid(pid5, NULL, 0);
        //printf("[INFO] Validator80 (pid5) terminated during cleanup.\n");
        sem_post(print_sem);
    }
    shmctl(shmid, IPC_RMID, NULL); // Remove shared memory 1
    
    //logwrite("Shared Memory 1 deleted\n");

    trans_Pool = NULL; // Set pointer to NULL after deletion

    pthread_cancel(input_thread);
    
    
    
    //logwrite("Shared Memory 2 deleted\n");
    
    

    msgctl(mqid, IPC_RMID, NULL);
    
    //logwrite("Message Queue deleted\n");
    
    if (unlink("/tmp/VALIDATOR_INPUT") == 0) {
        //logwrite("Named Pipe VALIDATOR_INPUT deleted\n");
    } else {
        perror("Failed to delete named pipe");
    }

    logwrite("CONTROLLER: Dumping the Ledger\n");
    print_blockchain();
    shmctl(shmid2, IPC_RMID, NULL); // Remove shared memory 2
    ledger = NULL;
    logwrite("CONTROLLER: CLOSING SIMULATION\n");
    destroy_log_things();
    destroy_trans_sem();
    destroy_block_sem();

    
    exit(0);
}



void *validator_aux(){
    int validator60 = 0;
    int validator80 = 0;

    int shmid = shmget(key1, sizeof(transactions_Pool), 0777);
    if (shmid == -1) {
        perror("Error: Unable to access shared memory");
        return 0;
    }

    trans_Pool = (transactions_Pool *)shmat(shmid, NULL, 0);
    if (trans_Pool == (void *)-1) {
        perror("Error: Unable to attach shared memory");
        return 0;
    }

    while(1){
    
        if(trans_Pool != NULL){
            
            if (sem_wait(sem_transactions) == -1) {
                    perror("sem_wait failed");
                    return NULL;
            } 

            int size_actual = trans_Pool->count;
            int total_size = trans_Pool->pool_size;
            
            sem_post(sem_transactions);

            if (size_actual >= total_size * 0.6 && validator60 ==0) {
                fflush(stdout);
                validator60 = 1;
                pid4 = fork();
                if (pid4 < 0) {
                    printf("Error: fork not executed correctly\n");
                    return NULL;
                } else if (pid4 == 0) {
                    logwrite("VALIDATOR: TRANSACTIONS POOL 60% FULL\n");
                    char buffer10[128];
                    snprintf(buffer10, sizeof(buffer10), "VALIDATOR 2: READY FOR WORK");
                    logwrite(buffer10);
                    validator(tam,2); 
                    continue;
                }

            }

            if (size_actual >= total_size * 0.8 && validator80 == 0) {
                validator80 = 1;
                
                pid5 = fork();
                if (pid5 < 0) {
                    printf("Error: fork not executed correctly\n");
                    return NULL;
                } else if (pid5 == 0) {

                    logwrite("VALIDATOR: TRANSACTIONS POOL 80% FULL\n");
                    char buffer9[128];
                    snprintf(buffer9, sizeof(buffer9), "VALIDATOR 3: READY FOR WORK");
                    logwrite(buffer9);
                    validator(tam, 3); 
                    continue;
                }

            }
            if(size_actual < total_size * 0.4 && validator60){
                logwrite("VALIDATOR: TRANSACTIONS POOL 40% FULL\n");
                
                if(validator80){
                    kill(pid5, SIGTERM);
                    waitpid(pid5, NULL, 0);
                    char buffer[128];
                    snprintf(buffer, sizeof(buffer), "VALIDATOR 3: TERMINATING\n");
                    logwrite(buffer);

                    validator80 = 0;
                }

                if(validator60){
                    kill(pid4, SIGTERM);
                    waitpid(pid4, NULL, 0);
                    char buffer[128];
                    snprintf(buffer, sizeof(buffer), "VALIDATOR 2: TERMINATING\n");
                    logwrite(buffer);
                    
                    validator60 = 0;
                }
                
                
            }
        }
        //pause();

    }
    
}
void print_blockchain() {
    if (ledger == NULL) {
        printf("Ledger is NULL.\n");
        return;
    }

    sem_wait(print_sem);
    printf("=================== Blockchain Ledger ===================\n");

    for (int i = 0; i < ledger->count; i++) {
        block *blk = &ledger->blocos[i];
        printf("||---- Block %03d --\n", i);
        printf("Block ID: BLOCK-%d-%d\n", blk->block_id, i);
        printf("Previous Hash:\n%s\n", blk->previous_hash);
        printf("Block Timestamp: %ld\n ", blk->timestamp);
        printf("Nonce: %u\n", blk->nonce);
        printf("Transactions:\n");

        for (int j = 0; j < blk->num_transactions; j++) {
            transaction *tx = &blk->transactions[j];
            printf(" [%d] ID: %s | Reward: %d | Value: %.2f | Timestamp: %ld \n", 
                j, tx->tx_id, tx->reward, (float)tx->value, tx->timestamp);
        }

        printf("----------------------------------------------------------\n");
    }
    sem_post(print_sem);
}



void handle_ctrl_l() {
    printf("VALIDATOR: SIGNAL SIGUSR1 RECEIVED\n");
    print_blockchain();
}

void* listen_for_ctrl_l() {
    struct termios oldt, newt;
    char ch;

    // Save old terminal settings
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    // Disable canonical mode and echo
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    while (1) {
        read(STDIN_FILENO, &ch, 1);
        if (ch == 12) { // ASCII code for Ctrl+L
            handle_ctrl_l();
        }
    }

    // Restore original settings when done (not reachable unless thread ends)
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return NULL;
}



int main(int argc, char *argv[]) {
    
    main_pid = getpid();
    val_aux = 0;
    init_log_things();
    init_trans_sem();
    init_block_sem();

    logwrite("CONTROLLER: DEI_CHAIN SIMULATOR STARTING\n");

    pthread_create(&input_thread, NULL, listen_for_ctrl_l, NULL);

    print_sem = sem_open("/print_sync", O_CREAT, 0666, 1);
    sem_write = sem_open("/write_sync", O_CREAT, 0644, 1); 
    if (print_sem == SEM_FAILED) {
        perror("sem_open");
        exit(1);
    }

    char fich[BUFFER];
    if(argc != 2){
        printf("comando: %s {config-file}\n", argv[0]);
        return 1;
    }
    strcpy(fich, argv[1]);

    int NUM_MINERS, TX_POOL_SIZE, TRANSACTIONS_BLOCK, BLOCKCHAIN_BLOCKS;
    // Open the file for reading
    FILE *file = fopen(fich, "r");
    if (file == NULL) {
        printf("Erro: Não foi possível abrir o ficheiro.\n");
        return 1;
    }
    int i;
    for(i = 0; i < 4; i++){
        char linha[BUFFER];
        fgets(linha, BUFFER, file);
        if(i == 0){
            NUM_MINERS = atoi(linha);
        }else if(i == 1){
            TX_POOL_SIZE = atoi(linha);
        }else if(i == 2){
            TRANSACTIONS_BLOCK = atoi(linha);
        }else if(i == 3){
            BLOCKCHAIN_BLOCKS = atoi(linha);
        }
    }

    if(i != 4){
        printf("Erro na leitura do ficheiro!\n");
        exit(-1);
    }
    fclose(file);

    /*
    printf("Variables assigned successfully:\n");
    printf("NUM_MINERS = %d\n", NUM_MINERS);
    printf("TX_POOL_SIZE = %d\n", TX_POOL_SIZE);
    printf("TRANSACTIONS_BLOCK = %d\n", TRANSACTIONS_BLOCK);
    printf("BLOCKCHAIN_BLOCKS = %d\n", BLOCKCHAIN_BLOCKS);
    */

    key_t key = ftok("teste", 65);
    mqid = msgget(key, IPC_CREAT | 0777);

    if (mqid == -1) {
        printf("msgget didnt process correctly");
        exit(1);
    }else{
        logwrite("CONTROLLER: MESSAGE QUEUE CREATED\n");
    }

    if (mkfifo("/tmp/VALIDATOR_INPUT", 0666) == -1) {
        if (errno != EEXIST) {
            perror("Error creating named pipe");
            exit(1);
        }
    } else {
        logwrite("CONTROLLER: NAMED PIPE CREATED\n");
    }

    // Shared memory for Transaction Pool

    if ((shmid = shmget(key1, TX_POOL_SIZE * sizeof(transactions_Pool), IPC_CREAT | 0777)) == -1) {
        perror("Error: in shmget - transactions_Pool");
        return 1;
    }
    trans_Pool = (transactions_Pool *)shmat(shmid, NULL, 0);
    if (trans_Pool == (void *)-1) {
        perror("Error: in shmat - transactions_Pool");
        return 1;
    }
    logwrite("CONTROLLER: SHM_TX_POOL CREATED\n");

    trans_Pool->pool_size = TX_POOL_SIZE;
    trans_Pool->max_trans_per_block = TRANSACTIONS_BLOCK;
    trans_Pool->count = 0;



    for (int i = 0; i < TX_POOL_SIZE; i++) {
        trans_Pool->transactions[i].empty = 1;
        trans_Pool->transactions[i].age = 0;   
    }

    tam = sizeof(int) * 3 + sizeof(time_t) + sizeof(unsigned int) + HASH_SIZE * 2 + sizeof(transaction) * trans_Pool->max_trans_per_block;

    if ((shmid2 = shmget(key2, sizeof(blockchain_Ledger) * BLOCKCHAIN_BLOCKS, IPC_CREAT | 0777)) == -1) {
        perror("Error: in shmget - blockchain_Ledger");
        return 1;
    }
    
    ledger = (blockchain_Ledger *)shmat(shmid2, NULL, 0);
    if (ledger == (void *)-1) {
        perror("Error: in shmat - blockchain_Ledger");
        return 1;
    }

    logwrite("CONTROLLER: SHM_LEDGER CREATED\n");

    pthread_create(&thread_validatores_aux, NULL, validator_aux, NULL);
    

    ledger->count = 0;
    ledger->tam = BLOCKCHAIN_BLOCKS;

    pid1 = fork();
    if (pid1 < 0) {
        printf("Error: fork not executed correctly\n");
        return -1;
    } else if (pid1 == 0) {
        miner(NUM_MINERS);
        exit(0); 
    }

    pid2 = fork();
    if (pid2 < 0) {
        printf("Error: fork not executed correctly\n");
        return -1;
    } else if (pid2 == 0) {
        val_aux +=1;
        logwrite("CONTROLLER: PROCESS VALIDATOR CREATED\n");
        char buffer8[128];
        snprintf(buffer8, sizeof(buffer8), "VALIDATOR %d: READY FOR WORK\n", val_aux);
        logwrite(buffer8);
        validator(tam, val_aux); 
        
        exit(0); 
    }

    pid3 = fork();
    if (pid3 < 0) {
        printf("Error: fork not executed correctly\n");
        return -1;
    } else if (pid3 == 0) {
        logwrite("CONTROLLER: PROCESS STATISTICS CREATED\n");
        statistics();
        exit(0); 
    }

    

    signal(SIGINT, clean);
    signal(SIGTERM, clean); 


    pause();
    return 0;
}