# 🔐 SecurePass - Password Strength & Breach Monitoring System

SecurePass is a **C++ console-based cybersecurity project** developed as part of a **Data Structures and Algorithms (DSA)** course. The system analyzes password security by checking strength, detecting weak/common passwords, and verifying whether a password exists in a breach database.

It demonstrates the practical implementation of core DSA concepts such as **hash tables, linked lists, queues, arrays, vectors, and sorting algorithms** in a real-world cybersecurity scenario.

---

## 🎯 Project Objectives

- Develop a password breach monitoring system in C++
- Check if a password exists in a breach database
- Analyze password strength based on complexity and length
- Detect commonly used weak passwords using linear search
- Use hashing for fast password lookup
- Handle collisions using linked list chaining
- Maintain password check history using circular queue
- Perform bulk password checking from file
- Apply DSA concepts in a cybersecurity application

---

## 🚨 Problem Statement

Users often choose weak or reused passwords that may already exist in data breaches. Attackers exploit these leaked passwords to compromise accounts. Manually checking passwords against large datasets is inefficient. SecurePass solves this by providing an efficient system for breach detection and password security analysis.

---

## 📌 Features

- 🔍 Single password security check
- 📂 Bulk password checking from file (`my_passwords.txt`)
- 🧠 Password strength scoring system
- 🚫 Breach detection using hash table
- ⚡ Fast lookup using hashing
- 🔗 Collision handling using linked lists
- 📊 Password statistics and reports
- 🕘 Recent password history (circular queue)
- 💡 Security recommendations
- 🏆 Strong password generation

---

## 🛠️ Tech Stack

- **Language:** C++
- **Type:** Console Application
- **IDE/Compiler:** Code::Blocks / DevC++ / Visual Studio / GCC
- **Storage:** File handling (text files)

---

## 📁 Project Files

| File Name | Description |
|-----------|-------------|
| `BreachMonitor.cpp` | Main source code file |
| `breached.txt` | Stores breached passwords database |
| `my_passwords.txt` | Input file for bulk password checking |

---

## 🧠 Data Structures Used

- **Array:** Store weak passwords, suggestions, and fixed-size data
- **Linked List:** Handle hash collisions
- **Hash Table:** Fast breach password lookup
- **Circular Queue:** Store recent password history
- **Vector:** Dynamic storage for bulk results
- **Structure:** Store suggestions and hash nodes

---

## ⚙️ Algorithms Used

- **Hashing:** Fast password indexing and search
- **Linear Search:** Weak password detection
- **Bubble Sort:** Sorting suggestions by priority

---

## 🔐 Password Scoring System

| Criteria | Score |
|----------|------|
| Length ≥ 12 | +30 |
| Uses 3–4 character types | +20 to +30 |
| Common weak password | -20 |
| Found in breach database | -30 |

### Rating Scale:
- 80–100 → Excellent
- 60–79 → Good
- 40–59 → Fair
- 20–39 → Weak
- 0–19 → Critical

---

## 📂 System Workflow

1. Load weak passwords into array  
2. Load breached passwords from file  
3. Insert passwords into hash table  
4. Display menu  
5. User selects an option  
6. System performs analysis  
7. Results displayed  
8. Repeat until exit  

---

## 📊 Functional Modules

- Check single password
- Bulk password checking
- Generate strong password
- View breach statistics
- View password history
- View system statistics
- Show security recommendations

---

## 📌 Example Output

- Password strength score
- Breach status (Safe / Breached)
- Weakness and strengths report
- Improvement suggestions
- Password category (Weak / Fair / Strong)

---

## 📸 Screenshots

(Add your screenshots here)

Example:
```md
![Main Menu](assets/main_menu.png)
![Password Check](assets/password_check.png)
