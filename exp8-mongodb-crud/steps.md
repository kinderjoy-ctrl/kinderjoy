# Experiment 8 — MongoDB CRUD Operations (Web App)

## Aim

Design and implement a basic CRUD (Create, Read, Update, Delete) operations system using MongoDB.

---

## What We Are Building

A simple **Items Manager** web app where you can:

- **Add** items (with a name and description)
- **View** all items on the page
- **Edit** an item using a popup prompt
- **Delete** an item

The app uses **Node.js + Express** as the server and **MongoDB** as the database.

---

## Prerequisites

- **Node.js** installed (download from https://nodejs.org)
- **MongoDB** installed and running (download from https://www.mongodb.com/try/download/community)

---

## Files in This Project

| File               | Purpose                                    |
| ------------------ | ------------------------------------------ |
| `package.json`     | Lists project dependencies                 |
| `server.js`        | Backend server with API routes             |
| `public/index.html`| Frontend page with form and item list      |

---

## Steps to Run

### Step 1 — Start MongoDB

1. Open a terminal/command prompt.
2. Run the MongoDB server:
   ```
   mongod
   ```
3. Keep this terminal open. MongoDB is now running on port `27017`.

> **Note:** On Windows, if `mongod` is not recognized, you may need to add MongoDB's `bin` folder to your system PATH, or run it from `C:\Program Files\MongoDB\Server\7.0\bin\mongod.exe`.

### Step 2 — Install Dependencies

1. Open a **new** terminal/command prompt.
2. Navigate to this experiment folder:
   ```
   cd path\to\exp8-mongodb-crud
   ```
3. Run:
   ```
   npm install
   ```
4. This will download `express` and `mongoose` packages.

### Step 3 — Start the Server

1. In the same terminal, run:
   ```
   npm start
   ```
2. You should see:
   ```
   Connected to MongoDB
   Server running on http://localhost:3000
   ```

### Step 4 — Open the App

1. Open your browser and go to: `http://localhost:3000`
2. You will see the **MongoDB CRUD App** page.

---

## How to Perform CRUD Operations

### CREATE — Add a New Item

1. Type an item name in the **Item Name** field (e.g., `Notebook`).
2. Type a description in the **Description** field (e.g., `A ruled notebook`).
3. Click the **Add** button.
4. The item will appear in the list below.

### READ — View All Items

- All items are automatically displayed on the page when it loads.
- Each item shows its name and description.

### UPDATE — Edit an Item

1. Click the **Edit** link next to any item.
2. A prompt box will appear asking for a new name — type the updated name.
3. Another prompt will ask for a new description — type the updated description.
4. The item will be updated in the list.

### DELETE — Remove an Item

1. Click the **Delete** link next to any item.
2. A confirmation box will ask "Delete this item?"
3. Click **OK** to confirm.
4. The item will be removed from the list.

---

## How It Works (Simple Explanation)

1. **server.js** creates an Express server and connects to MongoDB using Mongoose.
2. It defines a `Schema` (structure) for items — each item has a `name`, `description`, and `createdAt` date.
3. Four API routes handle the CRUD operations:
   - `POST /api/items` → Creates a new item
   - `GET /api/items` → Reads all items
   - `PUT /api/items/:id` → Updates an item by ID
   - `DELETE /api/items/:id` → Deletes an item by ID
4. **index.html** uses JavaScript (`XMLHttpRequest`) to call these APIs and update the page without reloading.

---

## API Endpoints

| Method   | URL               | What It Does        |
| -------- | ----------------- | ------------------- |
| `POST`   | `/api/items`      | Add a new item      |
| `GET`    | `/api/items`      | Get all items       |
| `PUT`    | `/api/items/:id`  | Update an item      |
| `DELETE` | `/api/items/:id`  | Delete an item      |
