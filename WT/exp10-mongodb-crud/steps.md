# Experiment 10 — MongoDB CRUD Operations (Command Line)

## Aim

Design and implement a basic CRUD (Create, Read, Update, Delete) operations system using MongoDB.

---

## What We Are Building

A **Node.js script** that connects to MongoDB and performs all four CRUD operations on a `products` collection — directly from the terminal. There is no web page in this experiment; everything runs in the command line.

The script will:

1. **Create** — Insert 4 products into the database
2. **Read** — Fetch all products, filter by category, and find a single product
3. **Update** — Change the price of a product
4. **Delete** — Remove a product
5. **Display** the final state of the database

---

## Prerequisites

- **Node.js** installed (download from https://nodejs.org)
- **MongoDB** installed and running

---

## Files in This Project

| File           | Purpose                                      |
| -------------- | -------------------------------------------- |
| `package.json` | Lists the project dependency (mongodb driver)|
| `crud.js`      | The main script that performs CRUD operations |

---

## Steps to Run

### Step 1 — Start MongoDB

1. Open a terminal/command prompt.
2. Run:
   ```
   mongod
   ```
3. Keep this terminal open.

### Step 2 — Install Dependencies

1. Open a **new** terminal.
2. Navigate to this experiment folder:
   ```
   cd path\to\exp10-mongodb-crud
   ```
3. Run:
   ```
   npm install
   ```
4. This downloads the `mongodb` driver package.

### Step 3 — Run the Script

1. In the same terminal, run:
   ```
   npm start
   ```
2. You will see the output of all CRUD operations printed in the terminal.

---

## Expected Output

When you run the script, you should see something like this:

```
Connected to MongoDB

--- CREATE ---
Inserted 4 documents

--- READ ALL ---
  Laptop - $999
  Phone - $699
  Desk Chair - $249
  Headphones - $149

--- READ FILTERED (Electronics) ---
  Laptop - $999
  Phone - $699
  Headphones - $149

--- READ ONE ---
  Found: Laptop - $999

--- UPDATE ---
  Laptop new price: $899

--- DELETE ---
  Deleted Headphones

--- FINAL STATE ---
  Laptop - $899
  Phone - $699
  Desk Chair - $249

Done.
```

---

## Understanding Each CRUD Operation

### CREATE — Inserting Documents

The script inserts 4 products into the `products` collection using `insertMany()`:

```javascript
await col.insertMany([
    { name: 'Laptop', price: 999, category: 'Electronics' },
    { name: 'Phone', price: 699, category: 'Electronics' },
    { name: 'Desk Chair', price: 249, category: 'Furniture' },
    { name: 'Headphones', price: 149, category: 'Electronics' }
]);
```

- `insertMany()` adds multiple documents at once.
- Each document is like a row in a table, but stored as a JSON object.

### READ — Fetching Documents

Three types of reads are shown:

```javascript
// Read all documents
var all = await col.find({}).toArray();

// Read with a filter (only Electronics)
var filtered = await col.find({ category: 'Electronics' }).toArray();

// Read a single document
var one = await col.findOne({ name: 'Laptop' });
```

- `find({})` with empty filter returns all documents.
- `find({ category: 'Electronics' })` returns only matching documents.
- `findOne()` returns just one matching document.

### UPDATE — Modifying a Document

```javascript
await col.updateOne(
    { name: 'Laptop' },           // find this document
    { $set: { price: 899 } }      // change its price to 899
);
```

- `updateOne()` updates the first matching document.
- `$set` tells MongoDB which fields to change.

### DELETE — Removing a Document

```javascript
await col.deleteOne({ name: 'Headphones' });
```

- `deleteOne()` removes the first document that matches the filter.

---

## Key MongoDB Methods Summary

| Method         | What It Does                        |
| -------------- | ----------------------------------- |
| `insertMany()` | Adds multiple documents             |
| `find({})`     | Gets all documents                  |
| `find(filter)` | Gets documents matching the filter  |
| `findOne()`    | Gets one matching document          |
| `updateOne()`  | Updates one matching document       |
| `deleteOne()`  | Deletes one matching document       |
| `deleteMany()` | Deletes all matching documents      |
