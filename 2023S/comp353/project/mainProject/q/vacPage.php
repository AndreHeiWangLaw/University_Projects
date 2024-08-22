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

// Delete
if (isset($_GET['delete'])) {
    $medicareCardNumberToDelete = $_GET['medicareCardNumber'];
    $doseNumberToDelete = $_GET['doseNumber'];

    // Delete query
    $delete_query = "DELETE FROM Vaccination
                     WHERE MedicareCardNumber = ? AND DoseNumber = ?";

    // Prepare  
    $stmt = $conn->prepare($delete_query);

    // Bind  
    $stmt->bind_param("ss", $medicareCardNumberToDelete, $doseNumberToDelete);

    // Execute  
    if ($stmt->execute()) {
        echo "Record deleted successfully.";
    } else {
        echo "Error deleting record: " . $stmt->error;
    }
    
    // Clear URL parameters after deletion
    unset($_GET['delete']);
    unset($_GET['medicareCardNumber']);
    unset($_GET['doseNumber']);

    // Close  
    $stmt->close();
}


// Edit
if (isset($_POST['edit_vaccination'])) {
    $newType = $_POST['new_type'];
    $newDate = $_POST['new_date'];
    $medicareCardNumber = $_POST['medicareCardNumber'];
    $doseNumToEdit = $_POST['doseNum'];
	
	// Edit query
    $update_query = "UPDATE Vaccination 
                     SET Type = '$newType', Date = '$newDate'
                     WHERE MedicareCardNumber = '$medicareCardNumber' AND DoseNumber = '$doseNumToEdit'";
    
    if ($conn->query($update_query)) {
        echo "Record updated successfully.";
    } else {
        echo "Error updating record: " . $conn->error;
    }
}

// Display
$select_query = "SELECT * FROM Vaccination";
$result = $conn->query($select_query);
?>

<!DOCTYPE html>
<html>
<head>
    <title>Vaccination</title>
</head>
<body>
<h1>Vaccination</h1>


<!-- Display -->
<h2>Current Vaccinations</h2>
<!-- Search Form -->
<form method="get">
    Search by<br>
    Medicare Card Number: <input type="text" name="searchMedicareCardNumber"> <br>
    Dose Number: <input type="text" name="searchDoseNumber"><br>
    <input type="submit" name="search" value="Search">
    <input type="submit" name="reset" value="Reset">
</form><br>
<table border="1">
    <tr>
        <th>Dose Number</th>
        <th>Date</th>
        <th>Type</th>
        <th>Medicare Card Number</th>
        <th>Action</th>
    </tr>
    <?php
    if (isset($_GET['search'])) {
        $searchMedicareCardNumber = $_GET['searchMedicareCardNumber'];
        $searchDoseNumber = $_GET['searchDoseNumber'];
        
        // query,  search criteria
        $search_query = "SELECT * FROM Vaccination
                         WHERE MedicareCardNumber LIKE '%$searchMedicareCardNumber%'
                         AND DoseNumber LIKE '%$searchDoseNumber%'";
        
        $result = $conn->query($search_query);
    } elseif (isset($_GET['reset'])) {
        // clear search fields
        $_GET['searchMedicareCardNumber'] = "";
        $_GET['searchDoseNumber'] = "";
    }
    while ($row = $result->fetch_assoc()) {
        echo "<tr>";
        echo "<td>" . $row['DoseNumber'] . "</td>";
        echo "<td>" . $row['Date'] . "</td>";
        echo "<td>" . $row['Type'] . "</td>";
        echo "<td>" . $row['MedicareCardNumber'] . "</td>";
        echo "<td><a href='vacPage.php?delete=true&medicareCardNumber=" . $row['MedicareCardNumber'] . "&doseNumber=" . $row['DoseNumber'] . "'>Delete</a></td>";
        echo "</tr>";
    }
    ?>
</table>
<br>

<!-- Create -->
<h2>Create a Vaccination</h2>
<form method="post" action="vacPage.php">
    Medicare Card Number: <input type="text" name="medicareCardNumber"><br>
    Dose Number: <input type="text" name="doseNumber"><br>
    Type: <input type="text" name="type"><br>
    Date: <input type="date" name="date"<br><br>
    <input type="submit" name="add_vaccination" value="Confirm">
</form>
<br>

<!-- Edit Form -->
<h2>Edit a Vaccination Type</h2>
<form method="post" action="vacPage.php">
    Medicare Card Number: <input type="text" name="medicareCardNumber"><br>
    Dose Number: <input type="text" name="doseNum"><br>
    <br>
    Change New Type of Infection: <input type="text" name="new_type"><br>
    Change New Date: <input type="date" name="new_date"><br>
    <input type="submit" name="edit_vaccination" value="Confirm"><br><br><br>
</form>

<!-- return to index.php -->
<a href="index.php">Go Back to Index Page</a>

</body>
</html>
