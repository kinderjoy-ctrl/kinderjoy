<?php
include 'db.php';

$id = intval($_GET['id']);
$result = $conn->query("SELECT * FROM students WHERE id=$id");
$student = $result->fetch_assoc();
$conn->close();
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Edit Student</title>
    <link rel="stylesheet" href="style.css">
</head>
<body>
    <div class="container">
        <h1>Edit Student</h1>
        <form action="process.php" method="POST">
            <input type="hidden" name="id" value="<?php echo $student['id']; ?>">
            <div class="form-row">
                <input type="text" name="name" value="<?php echo $student['name']; ?>" required>
                <input type="email" name="email" value="<?php echo $student['email']; ?>" required>
                <input type="text" name="course" value="<?php echo $student['course']; ?>" required>
                <button type="submit" name="action" value="update">Update</button>
            </div>
        </form>
        <br>
        <a href="index.php">← Back</a>
    </div>
</body>
</html>
