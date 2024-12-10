
# Optimization Project

 This project was developed as part of a university course on **Parallel Programming**. The primary objective is to explore and implement optimization techniques in C, both in sequential and parallelized contexts, while maintaining accuracy and efficiency.

## Project Overview

This project focuses on optimizing a dummy application that simulates real-world data processing. The goal was to improve runtime performance through both algorithmic and parallel computing strategies. The project involves:
- Reading and processing datasets containing IDs and numerical values.
- Filtering and combining data based on specific rules.
- Using advanced data structures like hash tables for efficient lookups.
- Parallelizing computationally intensive tasks using OpenMP.

The results are saved in a CSV file with processed and sorted data.

## Repository Structure

The repository is organized into the following directories:

```
optimization_project/
├── db/                # Contains database files and generator script
│   ├── db.c           # Code to generate the dataset
│   ├── A.txt          # Dataset A
│   ├── B.txt          # Dataset B
│   ├── ids.txt        # Dataset of IDs
├── src/               # Source code files for the project
│   ├── seq.c          # Baseline sequential implementation
│   ├── main.c         # Optimized implementation using hash tables and pre-filtering
│   ├── mainParalel.c  # Parallelized implementation using OpenMP
│   ├── filters.c      # Filtering logic
│   ├── ids.c          # ID processing and hash table logic
├── doc/               # Documentation directory
│   ├── solution.md    # Detailed explanation of the solution
│   ├── performance.md # Performance analysis
├── tutorial/          # Compilation and execution tutorial
│   ├── tutorial.md    # Step-by-step guide for setup and usage
└── README.md          # Repository overview (this file)
```

## How to Use

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/your-username/optimization_project.git
   cd optimization_project
   ```

2. **Compile the Code**:
   Follow the instructions in the `tutorial/tutorial.md` file to compile and execute the project.

3. **Understand the Implementation**:
   Refer to the `doc/solution.md` file for an in-depth explanation of the project’s implementation and optimization techniques.

## About

This project demonstrates the application of parallel programming concepts, including optimization techniques and the trade-offs involved in parallelizing computations. It highlights the benefits and challenges of parallel execution while showcasing algorithmic improvements for sequential processing.
