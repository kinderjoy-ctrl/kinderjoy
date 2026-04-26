<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>PHP MySQL CRUD</title>
    <link rel="stylesheet" href="style.css">
</head>
<body>

    <div class="container">
        <h1>Student Records</h1>

        <form action="process.php" method="POST">
            <input type="hidden" name="id" id="editId">
            <div class="form-row">
                <input type="text" name="name" placeholder="Student Name" required>
                <input type="email" name="email" placeholder="Email" required>
                <input type="text" name="course" placeholder="Course" required>
                <button type="submit" name="action" value="add">Add</button>
            </div>
        </form>

        <table>
            <thead>
                <tr>
                    <th>ID</th>
                    <th>Name</th>
                    <th>Email</th>
                    <th>Course</th>
                    <th>Actions</th>
                </tr>
            </thead>
            <tbody>
                <?php
                include 'db.php';
                $result = $conn->query("SELECT * FROM students ORDER BY id DESC");

                if ($result->num_rows > 0) {
                    while ($row = $result->fetch_assoc()) {
                        echo "<tr>
                            <td>{$row['id']}</td>
                            <td>{$row['name']}</td>
                            <td>{$row['email']}</td>
                            <td>{$row['course']}</td>
                            <td>
                                <a href='edit.php?id={$row['id']}' class='btn-edit'>Edit</a>
                                <a href='process.php?action=delete&id={$row['id']}' class='btn-delete' onclick='return confirm(\"Delete?\")'>Delete</a>
                            </td>
                        </tr>";
                    }
                } else {
                    echo "<tr><td colspan='5'>No records found</td></tr>";
                }

                $conn->close();
                ?>
            </tbody>
        </table>
    </div>

</body>
</html>
