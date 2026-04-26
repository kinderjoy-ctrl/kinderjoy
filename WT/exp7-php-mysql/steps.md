cpn# Experiment 7 — PHP + MySQL CRUD Operations

## Aim

Create interactive web pages that fetch, display, and update data from MySQL databases dynamically based on user interactions using PHP.

---

## What We Are Building

A **Student Records** management system where you can:

- **Add** a new student (name, email, course)
- **View** all students in a table
- **Edit** a student's details
- **Delete** a student record

---

## Prerequisites

- **XAMPP** installed (it gives us Apache + MySQL + PHP)
- A web browser

---

## Files in This Project

| File          | Purpose                                      |
| ------------- | -------------------------------------------- |
| `setup.sql`   | Creates the database and table               |
| `db.php`      | Connects PHP to the MySQL database           |
| `index.php`   | Main page — shows the form and student table  |
| `edit.php`    | Page to edit a student's details             |
| `process.php` | Handles add, update, and delete actions      |
| `style.css`   | Styling for the pages                        |

---

## Steps to Run

### Step 1 — Start XAMPP

1. Open **XAMPP Control Panel**.
2. Click **Start** next to **Apache** and **MySQL**.
3. Both should turn green — this means the server is running.

### Step 2 — Copy the Project

1. Go to `C:\xampp\htdocs\` on your computer.
2. Copy the entire `exp7-php-mysql` folder into `htdocs`.
3. The path should look like: `C:\xampp\htdocs\exp7-php-mysql\`

### Step 3 — Create the Database

1. Open your browser and go to: `http://localhost/phpmyadmin`
2. Click on the **SQL** tab at the top.
3. Open the `setup.sql` file, copy its contents, and paste it into the SQL box.
4. Click **Go** to run it.
5. This creates a database called `student_db` with a `students` table and 3 sample records.

### Step 4 — Open the App

1. In your browser, go to: `http://localhost/exp7-php-mysql/`
2. You should see the **Student Records** page with a form and a table.

---

## How to Perform CRUD Operations

### CREATE — Add a New Student

1. On the main page, you will see three input fields: **Name**, **Email**, and **Course**.
2. Fill in the details, for example:
   - Name: `David Wilson`
   - Email: `david@example.com`
   - Course: `Mechanical`
3. Click the **Add** button.
4. The page will reload and the new student will appear in the table.

### READ — View All Students

- The table on the main page automatically shows all students from the database.
- Students are listed with their ID, Name, Email, Course, and action buttons.

### UPDATE — Edit a Student

1. In the table, click the **Edit** link next to any student.
2. You will be taken to the **Edit Student** page with the current details filled in.
3. Change any field (e.g., update the course from `Computer Science` to `Data Science`).
4. Click the **Update** button.
5. You will be redirected back to the main page with the updated data.

### DELETE — Remove a Student

1. In the table, click the **Delete** link next to any student.
2. A confirmation popup will ask "Delete?"
3. Click **OK** to confirm.
4. The student will be removed and the table will refresh.

---

## How It Works (Simple Explanation)

1. **db.php** connects to MySQL using `mysqli`.
2. **index.php** runs a `SELECT` query to get all students and displays them in a table.
3. When you fill the form and click **Add**, the data is sent to **process.php** which runs an `INSERT` query.
4. When you click **Edit**, **edit.php** loads that student's data using `SELECT WHERE id=...` and shows it in a form.
5. When you submit the edit form, **process.php** runs an `UPDATE` query.
6. When you click **Delete**, **process.php** runs a `DELETE` query.

---

## Key SQL Queries Used

```sql
-- Create
INSERT INTO students (name, email, course) VALUES ('Alice', 'alice@example.com', 'CS');

-- Read
SELECT * FROM students ORDER BY id DESC;

-- Update
UPDATE students SET name='Bob', email='bob@example.com', course='IT' WHERE id=1;

-- Delete
DELETE FROM students WHERE id=1;
```
