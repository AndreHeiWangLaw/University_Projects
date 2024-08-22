<?php



$server = 'localhost:3306';
$username = 'root';
$password = 'password';
$database = 'comp353';

try {
    $conn = new PDO("mysql.host=$server;dbname=$database;", $username, $password);
} catch(PDOException $e){
    die('Failed to Coonect (Check code): ' . $e->getMessage());
}

?>

