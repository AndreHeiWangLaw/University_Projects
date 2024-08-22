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


// Create
if (isset($_POST['add_vaccination'])) {
    $doseNumber = $_POST['doseNumber'];
    $date = $_POST['date'];
    $type = $_POST['type'];
    $medicareCardNumber = $_POST['medicareCardNumber'];

    $insert_query = "INSERT INTO Vaccination (DoseNumber, Date, Type, MedicareCardNumber)
                     VALUES ('$doseNumber', '$date', '$type', '$medicareCardNumber')";
    $conn->query($insert_query);
}

// Edit
if (isset($_POST['edit_vaccination'])) {
    $newType = $_POST['new_type'];
    $medicareCardNumber = $_POST['medicareCardNumber'];
    $dateOfVaccination = $_POST['dateOfVaccination'];

    $update_query = "UPDATE Vaccination SET Type = '$newType'
                     WHERE MedicareCardNumber = '$medicareCardNumber' AND Date = '$dateOfVaccination'";
    $conn->query($update_query);
}

// Delete
if (isset($_GET['delete'])) {
    $medicareCardNumber = $_GET['medicareCardNumber'];
    $dateOfVaccination = $_GET['dateOfVaccination'];

    $delete_query = "DELETE FROM Vaccination
                     WHERE MedicareCardNumber = '$medicareCardNumber' AND Date = '$dateOfVaccination'";
    $conn->query($delete_query);
}

// Display
$select_query = "SELECT * FROM Vaccination";
$result = $conn->query($select_query);
?>

<!DOCTYPE html>
<html>
<head>
    <title>Vaccination Management</title>
</head>
<body>
<h1>Vaccination Management</h1>

<!-- Create Form -->
<h2>Add Vaccination</h2>
<form method="post">
    Dose Number: <input type="text" name="doseNumber"><br>
    Date: <input type="date" name="date"><br>
    Type: <input type="text" name="type"><br>
    Medicare Card Number: <input type="text" name="medicareCardNumber"><br>
    <input type="submit" name="add_vaccination" value="Add Vaccination">
</form>

<!-- Display Vaccinations -->
<h2>Current Vaccinations</h2>
<table border="1">
    <tr>
        <th>Dose Number</th>
        <th>Date</th>
        <th>Type</th>
        <th>Medicare Card Number</th>
        <th>Action</th>
    </tr>
    <?php
    while ($row = $result->fetch_assoc()) {
        echo "<tr>";
        echo "<td>" . $row['DoseNumber'] . "</td>";
        echo "<td>" . $row['Date'] . "</td>";
        echo "<td>" . $row['Type'] . "</td>";
        echo "<td>" . $row['MedicareCardNumber'] . "</td>";
        echo "<td><a href='vaccination.php?delete=true&medicareCardNumber=" . $row['MedicareCardNumber'] . "&dateOfVaccination=" . $row['Date'] . "'>Delete</a></td>";
        echo "</tr>";
    }
    ?>
</table>

<!-- Edit Form -->
<h2>Edit Vaccination Type</h2>
<form method="post">
    Medicare Card Number: <input type="text" name="medicareCardNumber"><br>
    Date of Vaccination: <input type="date" name="dateOfVaccination"><br>
    New Type: <input type="text" name="new_type"><br>
    <input type="submit" name="edit_vaccination" value="Edit Vaccination Type">
</form>
</body>
</html>
