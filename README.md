# Constrained Channel Coding Toolkit (C++)

A C++ library for modeling **binary constrained channels**, computing **channel capacity**, and constructing **(n, k) codebooks** using a Finite State Machine (FSM) and Freiman–Wyner-inspired block construction.

---

## Features

- Finite State Machine (FSM) modeling of run-length constrained channels
- Automatic state generation (e.g. `0000, 000, ..., 1111`)
- Adjacency matrix construction
- Largest eigenvalue computation (Power Iteration)
- Channel capacity calculation:
  \[
  C = \log_2(\lambda)
  \]
- Generation of all valid binary sequences under constraints
- Block-based codeword construction
- Verification of existence of codes of type \((n, k)\)
- Automatic CSV export of:
  - FSM transitions
  - adjacency matrix
  - valid codewords
  - selected codebook
  - capacity results
- Automatic run folder creation (timestamped)

---

## Problem Definition

We consider a **binary channel with maximum run-length constraint** \(R\):

- No more than \(R\) identical bits in a row
- Example for \(R = 4\):

- ## 📁 Project Structure
- include/constrained/
types.h
states.h
fsm.h
generator.h
capacity.h
codebook.h
io_utils.h

src/
states.cpp
fsm.cpp
generator.cpp
capacity.cpp
codebook.cpp
io_utils.cpp
