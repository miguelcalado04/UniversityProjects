import random
import copy
import numpy as np
import gymnasium as gym 
import os
from multiprocessing import Process, Queue

# CONFIG
ENABLE_WIND = True
WIND_POWER = 15.0
TURBULENCE_POWER = 0.0
GRAVITY = -10.0
RENDER_MODE = 'human'
RENDER_MODE = None
EPISODES = 1000
STEPS = 500

NUM_PROCESSES = os.cpu_count()
evaluationQueue = Queue()
evaluatedQueue = Queue()


nInputs = 8
nOutputs = 2
SHAPE = (nInputs,12,nOutputs)
GENOTYPE_SIZE = 0
for i in range(1, len(SHAPE)):
    GENOTYPE_SIZE += SHAPE[i-1]*SHAPE[i]

POPULATION_SIZE = 100
NUMBER_OF_GENERATIONS = 100
PROB_CROSSOVER = 0.9


PROB_MUTATION = 0.05
STD_DEV = 0.8


ELITE_SIZE = 0


def network(shape, observation,ind):
    #Computes the output of the neural network given the observation and the genotype
    x = observation[:]
    for i in range(1,len(shape)):
        y = np.zeros(shape[i])
        for j in range(shape[i]):
            for k in range(len(x)):
                y[j] += x[k]*ind[k+j*len(x)]
        x = np.tanh(y)
    return x


def check_successful_landing(observation):
    #Checks the success of the landing based on the observation
    x = observation[0]
    vy = observation[3]
    theta = observation[4]
    contact_left = observation[6]
    contact_right = observation[7]

    legs_touching = contact_left == 1 and contact_right == 1

    on_landing_pad = abs(x) <= 0.2

    stable_velocity = vy > -0.2
    stable_orientation = abs(theta) < np.deg2rad(20)
    stable = stable_velocity and stable_orientation
 
    if legs_touching and on_landing_pad and stable:
        return True
    return False

def objective_function(observation):

    x = observation[0]
    y = observation[1]
    vx = observation[2]
    vy = observation[3]
    theta = observation[4]
    omega = observation[5]
   
    success = check_successful_landing(observation)

    #penalidades
    pos_penalty = abs(x) + abs(y)
    vel_penalty = abs(vx) + abs(vy)
    angle_penalty = abs(theta)
    rotation_penalty = abs(omega)
    
    if ENABLE_WIND: 
        #quanto menor o erro, maior a fitness
        fitness = -(pos_penalty + vel_penalty + angle_penalty * 0.5 + rotation_penalty * 0.2)
    
        if success: 
            fitness += 100
    else:
        fitness = -(pos_penalty + vel_penalty + angle_penalty * 0.8 + rotation_penalty * 0.4)
        
        if success: 
            fitness += 100
 
    
    return fitness, success

def simulate(genotype, render_mode = None, seed=None, env = None):
    #Simulates an episode of Lunar Lander, evaluating an individual
    env_was_none = env is None
    if env is None:
        env = gym.make("LunarLander-v3", render_mode =render_mode, 
        continuous=True, gravity=GRAVITY, 
        enable_wind=ENABLE_WIND, wind_power=WIND_POWER, 
        turbulence_power=TURBULENCE_POWER)    
        
    observation, info = env.reset(seed=seed)

    for _ in range(STEPS):
        prev_observation = observation
        #Chooses an action based on the individual's genotype
        action = network(SHAPE, observation, genotype)
        observation, reward, terminated, truncated, info = env.step(action)        

        if terminated == True or truncated == True:
            break
    
    if env_was_none:    
        env.close()

    return objective_function(prev_observation)

def evaluate(evaluationQueue, evaluatedQueue):
    #Evaluates individuals until it receives None
    #This function runs on multiple processes
    
    env = gym.make("LunarLander-v3", render_mode =None, 
        continuous=True, gravity=GRAVITY, 
        enable_wind=ENABLE_WIND, wind_power=WIND_POWER, 
        turbulence_power=TURBULENCE_POWER)    
    while True:
        ind = evaluationQueue.get()

        if ind is None:
            break
            
        ind['fitness'] = simulate(ind['genotype'], seed = None, env = env)[0]
                
        evaluatedQueue.put(ind)
    env.close()
    
def evaluate_population(population):
    #Evaluates a list of individuals using multiple processes
    for i in range(len(population)):
        evaluationQueue.put(population[i])
    new_pop = []
    for i in range(len(population)):
        ind = evaluatedQueue.get()
        new_pop.append(ind)
    return new_pop

def generate_initial_population():
    #Generates the initial population
    population = []
    for i in range(POPULATION_SIZE):
        #Each individual is a dictionary with a genotype and a fitness value
        #At this time, the fitness value is None
        #The genotype is a list of floats sampled from a uniform distribution between -1 and 1
        
        genotype = []
        for j in range(GENOTYPE_SIZE):
            genotype += [random.uniform(-1,1)]
        population.append({'genotype': genotype, 'fitness': None})
    return population

def parent_selection(population):
    # Seleção de pais usando torneio estocástico

    tournament_size = 10
    # Define o tamanho do torneio, ou seja, quantos indivíduos serão escolhidos aleatoriamente da população

    tournament = random.sample(population, tournament_size)
    # Seleciona aleatoriamente 'tournament_size' indivíduos da população sem reposição

    tournament.sort(key=lambda x: x['fitness'], reverse=True)
    # Ordena os indivíduos selecionados com base no valor de fitness (do melhor para o pior)

    selected = tournament[0]



    return copy.deepcopy(selected)
    # Retorna uma cópia profunda do indivíduo selecionado para evitar alterações acidentais no original


def crossover(p1, p2, alpha=0.5):
    # Gera um novo indivíduo (filho) combinando os genes dos pais p1 e p2
    # alpha controla o quanto o filho pode extrapolar os genes dos pais
    
    child_genotype = []  # Lista que armazenará o genótipo do filho
    
    for g1, g2 in zip(p1['genotype'], p2['genotype']):
        # Percorre os genes dos dois pais ao mesmo tempo
        
        beta = random.uniform(-alpha, 1 + alpha)
        # beta é um fator aleatório que permite interpolar ou até extrapolar os valores dos genes
        # Ex: se alpha = 0.5 → beta ∈ [-0.5, 1.5]
        
        gene = beta * g1 + (1 - beta) * g2
        # Calcula o gene do filho com base na combinação linear dos genes dos pais
        
        child_genotype.append(gene)
        # Adiciona o gene gerado ao genótipo do filho
    
    return {'genotype': child_genotype, 'fitness': None}
    # Retorna o novo indivíduo com fitness indefinido (a ser avaliado depois)



def mutation(p, generation):
    # Função de mutação que aplica perturbações gaussianas ao genótipo de um indivíduo 'p'
    # O desvio padrão da mutação diminui ao longo das gerações (simulated annealing)
    
    annealed_std = max(STD_DEV * (0.99 ** generation), 0.05)
    # Cálculo do desvio padrão com decaimento exponencial ao longo das gerações
    # Começa com STD_DEV e vai diminuindo a cada geração (annealing)
    # Garante que o desvio padrão nunca fique abaixo de 0.05 (exploração mínima)

    new_genotype = p['genotype'][:]
    # Cria uma cópia do genótipo original para não modificar o indivíduo original

    for i in range(len(new_genotype)):
        # Para cada gene do genótipo
        if random.random() < PROB_MUTATION:
            # Com uma certa probabilidade, aplica mutação
            new_genotype[i] += np.random.normal(0, annealed_std)
            # Adiciona um ruído gaussiano (normal) ao gene, com média 0 e desvio annealed_std

    return {'genotype': new_genotype, 'fitness': None}
    # Retorna um novo indivíduo com o genótipo modificado e fitness indefinido (a ser avaliado depois)

    
def survival_selection(population, offspring):
    #reevaluation of the elite
    offspring.sort(key = lambda x: x['fitness'], reverse=True)
    p = evaluate_population(population[:ELITE_SIZE])
    new_population = p + offspring[ELITE_SIZE:]
    new_population.sort(key = lambda x: x['fitness'], reverse=True)
    return new_population    
        
def evolution():
    #Create evaluation processes
    evaluation_processes = []
    for i in range(NUM_PROCESSES):
        evaluation_processes.append(Process(target=evaluate, args=(evaluationQueue, evaluatedQueue)))
        evaluation_processes[-1].start()
    
    #Create initial population
    bests = []
    population = list(generate_initial_population())
    population = evaluate_population(population)
    population.sort(key = lambda x: x['fitness'], reverse=True)
    best = (population[0]['genotype']), population[0]['fitness']
    bests.append(best)
    
    #Iterate over generations
    for gen in range(NUMBER_OF_GENERATIONS):
        offspring = []
        
        #create offspring
        while len(offspring) < POPULATION_SIZE:
            if random.random() < PROB_CROSSOVER:
                p1 = parent_selection(population)
                p2 = parent_selection(population)
                ni = crossover(p1, p2)

            else:
                ni = parent_selection(population)
                
            ni = mutation(ni, gen)
            offspring.append(ni)
            
        #Evaluate offspring
        offspring = evaluate_population(offspring)

        #Apply survival selection
        population = survival_selection(population, offspring)
        
        #Print and save the best of the current generation
        best = (population[0]['genotype']), population[0]['fitness']
        bests.append(best)
        print(f'Best of generation {gen}: {best[1]}')

    #Stop evaluation processes
    for i in range(NUM_PROCESSES):
        evaluationQueue.put(None)
    for p in evaluation_processes:
        p.join()
        
    #Return the list of bests
    return bests

def load_bests(fname):
    #Load bests from file
    bests = []
    with open(fname, 'r') as f:
        for line in f:
            fitness, shape, genotype = line.split('\t')
            bests.append((eval(fitness),eval(shape), eval(genotype)))
    return bests

if __name__ == '__main__':
    
    evolve = True
    render_mode = 'human'
    if evolve:
        seeds = [964, 952, 364, 913, 140, 726, 112, 631, 881, 844, 965, 672, 335, 611, 457, 591, 551, 538, 673, 437, 513, 893, 709, 489, 788, 709, 751, 467, 596, 976]
        for i in range(30):    
            random.seed(seeds[i])
            bests = evolution()
            with open(f'log{i}.txt', 'w') as f:
                for b in bests:
                    f.write(f'{b[1]}\t{SHAPE}\t{b[0]}\n')

                
    else:
        #validate individual
        bests = load_bests('log0.txt')
        b = bests[-1]
        SHAPE = b[1]
        ind = b[2]
            
        ind = {'genotype': ind, 'fitness': None}
            
            
        ntests = 1000

        fit, success = 0, 0
        for i in range(1,ntests+1):
            f, s = simulate(ind['genotype'], render_mode=render_mode, seed = None)
            fit += f
            success += s
        print(fit/ntests, success/ntests)