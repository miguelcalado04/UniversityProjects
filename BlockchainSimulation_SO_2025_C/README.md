# Operating Systems - DEIChain (Blockchain Simulation)

<img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/c/c-original.svg" width="60"/>

## Overview
DEIChain is a simplified blockchain simulation developed for the Operating Systems course.  
It focuses on concurrency, IPC, shared memory, and process synchronization.

---

## System Components

### Controller
- Initializes system from `config.cfg`
- Creates IPC (shared memory, pipes, message queue)
- Launches Miner, Validator, Statistics
- Handles `SIGINT` (shutdown) and `SIGUSR1` (ledger dump)

### Miner
- Uses multiple threads (`NUM_MINERS`)
- Builds blocks from transactions
- Executes Proof-of-Work (SHA-256)
- Sends blocks to Validator

### Validator
- Validates blocks (PoW + transactions + chain)
- Updates blockchain ledger
- Manages transaction pool and aging
- Scales dynamically based on load

### Statistics
- Receives updates via message queue
- Prints system statistics on signal or shutdown

### TxGen
- Generates transactions periodically
- Writes to shared transaction pool

---

## Data

### Transaction
- ID, Reward (1–3), Value, Timestamp

### Block
- ID, Previous Hash, Timestamp, Transactions, Nonce

### Shared Structures
- Transaction Pool (pending transactions)
- Blockchain Ledger (validated blocks)

---

## Proof-of-Work
- SHA-256 hashing
- Increment nonce until hash matches difficulty
- Difficulty depends on transaction rewards
- Limited by `POW_MAX_OPS`

---

## IPC Used
- Shared Memory → Pool + Ledger
- Named Pipe → Miner → Validator
- Message Queue → Validator → Statistics
- Semaphores/Mutex → synchronization
