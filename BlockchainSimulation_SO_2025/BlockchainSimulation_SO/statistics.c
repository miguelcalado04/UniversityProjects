//Cláudio Catarino 2022224320
//Miguel Calado 2022228235


#include "funcs.h"

int msgqid;
MinerStats* lista = NULL;  // Começa vazia
int total_miners = 0; // Quantos miners temos salvos
int count = 0;
time_t tempo_total = 0;
int total_blocos_validos = 0;
int total_blocos_invalidos = 0;
pthread_t cntrl_l_thread;


int compare_miner_id(const void *a, const void *b) {
    const MinerStats *m1 = (const MinerStats *)a;
    const MinerStats *m2 = (const MinerStats *)b;
    return m1->miner_id - m2->miner_id;
}

void print_stats() {
    sem_wait(print_sem);
    printf("\n===== Estatísticas Gerais =====\n");
    printf("Total number of blocks validated (both correct and incorrect): %d\n", count);
    printf("Total number of blocks in the Blockchain (valid): %d\n", total_blocos_validos);

    if (count > 0) {
        double media_tempo = (double)tempo_total / count;
        printf("Average time to verify a transaction: %.2f\n", media_tempo);
    } else {
        printf("Average time to verify a transaction: N/A (nenhum bloco processado)\n");
    }
    qsort(lista, total_miners, sizeof(lista[0]), compare_miner_id);

    printf("\n===== Estatísticas por Minerador =====\n");
    if(lista == NULL){printf("Sem estatísticas\n\n");}
    for (int i = 0; i < total_miners; i++) {
        printf("Miner ID: %d\n", lista[i].miner_id);
        printf("  Valid blocks: %d\n", lista[i].blocos_validos);
        printf("  Invalid blocks: %d\n", lista[i].blocos_invalidos);
        printf("  Total credits (recompensa): %d\n", lista[i].total_recompensa);
        printf("\n");
    }
    
    sem_post(print_sem);
}

void han_ctrl_l() {
    printf("STATISTICS: SIGNAL SIGUSR1 RECEIVED");
    print_stats();
    
}

void* ctrl_l() {
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
            han_ctrl_l();
        }
    }

    // Restore original settings when done (not reachable unless thread ends)
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return NULL;
}

void cleanb(){
    

    print_stats();
    //logwrite("Statistics thread closed. Exiting.\n");
    pthread_cancel(cntrl_l_thread);
    exit(0);
}


MinerStats* miner_search(int miner_id) {
    for (int i = 0; i < total_miners; i++) {
        if (lista[i].miner_id == miner_id) {
            return &lista[i]; // Já existe
        }
    }

    // Se não existe, cria um novo
    MinerStats* temp = realloc(lista, (total_miners + 1) * sizeof(MinerStats));
    if (temp == NULL) {
        fprintf(stderr, "Erro de alocação!\n");
        exit(1);
    }
    lista = temp;
    

    lista[total_miners].miner_id = miner_id;
    lista[total_miners].blocos_validos = 0;
    lista[total_miners].blocos_invalidos = 0;
    lista[total_miners].total_recompensa = 0;
    total_miners++;
    
    return &lista[total_miners - 1];
}

void atualizar(int miner_id, int valido, int recompensa) {
    MinerStats* m = miner_search(miner_id);
    if (valido) {
        m->blocos_validos++;
        m->total_recompensa += recompensa;
    } else {
        m->blocos_invalidos++;
    }
}



int statistics(){
    
    print_sem = sem_open("/print_sync", 0);
    if (print_sem == SEM_FAILED) {
        perror("sem_open");
        exit(1);
    }
    key_t key = ftok("teste", 65);
    msgqid = msgget(key, 0666);

    if (msgqid == -1) {
    perror("msgget");
    exit(1);
    }

    pthread_create(&cntrl_l_thread, NULL, ctrl_l, NULL);
    

    signal(SIGINT, cleanb);
    while(1){
        msg m;
        if (msgrcv(msgqid, &m, sizeof(msg) - sizeof(long), 0, 0) == -1) {
            perror("msgrcv");
            exit(1);
        }
    /*
        printf("Mensagem recebida:\n");
        printf("  Miner ID: %d\n", m.miner_id);
        printf("  Valido: %d\n", m.is_valid);
        printf("  Recompensa: %d\n", m.total_reward);
        printf("  Tempo Médio: %ld\n", m.tempo_medio);
    */

        tempo_total += m.tempo_medio;
        count += 1;
        if(m.is_valid){
            total_blocos_validos +=1;
        }else{
            total_blocos_invalidos +=1;
        }
        atualizar(m.miner_id,m.is_valid,m.total_reward);

    }
    
    pause();
    return 0;
}