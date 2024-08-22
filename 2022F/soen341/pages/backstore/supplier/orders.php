<?php
session_start();
if ((strcasecmp($_SESSION['role'], 'supplier') != 0) && (strcasecmp($_SESSION['role'], 'supervisor') != 0))
    echo "<script type='text/javascript'>document.location.replace(\"../../frontstore/index.php\");</script>";
?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Title</title>
</head>
<body>
<h1>orders page</h1>
</body>
</html>