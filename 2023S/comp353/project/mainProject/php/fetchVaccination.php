<?php


$host = 'fdc353.encs.concordia.ca';
$user = 'fdc353_1';
$pass = '353psswd';
$db = 'fdc353_1'; // replace 'your_database_name' with your actual database name

// Create connection
$conn = mysqli_connect($host, $user, $pass, $db);

// Check connection
if (!$conn) {
    die("Connection failed: " . mysqli_connect_error());
}
echo "Connected successfully<br>";

$sql = "SELECT * FROM Infection";
$result = $conn->query($sql);

if ($result->num_rows > 0) {
    while ($row = $result->fetch_assoc()) {
        echo "Date of Infection: " . $row["DateOfInfection"] . "<br>";
        echo "Medicare Card Number: " . $row["MedicareCardNumber"] . "<br>";
        echo "Infection Type: " . $row["InfectionType"] . "<br>";
        echo "<hr>";
    }
} else {
    echo "No infection records found.";
}
?>
