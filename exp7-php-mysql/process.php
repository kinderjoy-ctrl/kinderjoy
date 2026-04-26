<?php
include 'db.php';

if ($_SERVER['REQUEST_METHOD'] === 'POST' && $_POST['action'] === 'add') {
    $name = $conn->real_escape_string($_POST['name']);
    $email = $conn->real_escape_string($_POST['email']);
    $course = $conn->real_escape_string($_POST['course']);

    $conn->query("INSERT INTO students (name, email, course) VALUES ('$name', '$email', '$course')");
    header("Location: index.php");
    exit;
}

if ($_SERVER['REQUEST_METHOD'] === 'POST' && $_POST['action'] === 'update') {
    $id = intval($_POST['id']);
    $name = $conn->real_escape_string($_POST['name']);
    $email = $conn->real_escape_string($_POST['email']);
    $course = $conn->real_escape_string($_POST['course']);

    $conn->query("UPDATE students SET name='$name', email='$email', course='$course' WHERE id=$id");
    header("Location: index.php");
    exit;
}

if (isset($_GET['action']) && $_GET['action'] === 'delete' && isset($_GET['id'])) {
    $id = intval($_GET['id']);
    $conn->query("DELETE FROM students WHERE id=$id");
    header("Location: index.php");
    exit;
}

$conn->close();
?>
