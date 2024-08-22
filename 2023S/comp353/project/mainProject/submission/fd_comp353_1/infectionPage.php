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
if (isset($_POST['add_infection'])) {
    $dateOfInfection = $_POST['dateOfInfection'];
    $medicareCardNumber = $_POST['medicareCardNumber'];
    $infectionType = $_POST['infectionType'];

    $insert_query = "INSERT INTO Infection (DateOfInfection, MedicareCardNumber, InfectionType)
                     VALUES ('$dateOfInfection', '$medicareCardNumber', '$infectionType')";
    $conn->query($insert_query);
}

// Delete
if (isset($_GET['delete'])) {
    $medicareCardNumberToDelete = $_GET['medicareCardNumber'];
    $dateOfInfectionToDelete = $_GET['dateOfInfection'];

    // Delete query
    $delete_query = "DELETE FROM Infection
                     WHERE MedicareCardNumber = ? AND DateOfInfection = ?";
   
    // Prepare  
	$stmt = $conn->prepare($delete_query);
	
	// Bind  
    $stmt->bind_param("ss", $medicareCardNumberToDelete, $dateOfInfectionToDelete);

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
if (isset($_POST['edit_infection'])) {
    $newType = $_POST['newTypeToEdit'];
    $medicareCardNumber = $_POST['medicareCardNumber'];
    $dateToEdit = $_POST['date'];

	// Edit query
    $update_query = "UPDATE Infection 
						SET InfectionType = '$newType'
					WHERE 
						MedicareCardNumber = '$medicareCardNumber' AND
						DateOfInfection = '$dateToEdit'";

    // Execute  
    if ($conn->query($update_query)) {
        echo "Record updated successfully.";
    } else {
        echo "Error updating record: " . $conn->error;
    }
}

// Display
$select_query = "SELECT * FROM Infection";
$result = $conn->query($select_query);
?>

<!DOCTYPE html>
<html>
<head>
    <title>Infection</title>
</head>
<body>
<h1>Infection</h1>

<!-- Display -->
<h2>Current Infections</h2>
<!-- Search Form -->
<form method="get">
    Search by<br>
    Medicare Card Number: <input type="text" name="searchMedicareCardNumber"> <br>
    Date of Infection: <input type="date" name="searchDateOfInfection"> <br>
    <input type="submit" name="search" value="Search">
    <input type="submit" name="reset" value="Reset">
</form><br>
<table border="1">
    <tr>
        <th>Date of Infection</th>
        <th>Medicare Card Number</th>
        <th>Infection Type</th>
        <th>Action</th>
    </tr>
    <?php
    if (isset($_GET['search'])) {
        $searchMedicareCardNumber = $_GET['searchMedicareCardNumber'];
        $searchDateOfInfection = $_GET['searchDateOfInfection'];

		// query,  search criteria
        $search_query = "SELECT * FROM Infection
                 WHERE MedicareCardNumber LIKE '%$searchMedicareCardNumber%'
                   " . (empty($searchDateOfInfection) ? "" : "AND DateOfInfection LIKE '$searchDateOfInfection'");

        $result = $conn->query($search_query);
    } elseif (isset($_GET['reset'])) {
		// clear search fields
        $_GET['searchMedicareCardNumber'] = "";
	    	$_GET['searchDateOfInfection'] = "";
    }
    while ($row = $result->fetch_assoc()) {
        echo "<tr>";
        echo "<td>" . $row['DateOfInfection'] . "</td>";
        echo "<td>" . $row['MedicareCardNumber'] . "</td>";
        echo "<td>" . $row['InfectionType'] . "</td>";
        echo "<td><a href='infectionPage.php?delete=true&medicareCardNumber=" . $row['MedicareCardNumber'] . "&dateOfInfection=" . $row['DateOfInfection'] . "'>Delete</a></td>";
        echo "</tr>";
    }
    ?>
</table>
<br>

<!-- Create -->
<h2>Create an Infection</h2>
<form method="post" action="infectionPage.php">
    Medicare Card Number: <input type="text" name="medicareCardNumber"><br>
    Date of Infection: <input type="date" name="dateOfInfection"><br>
    Infection Type: <input type="text" name="infectionType"><br>
    <input type="submit" name="add_infection" value="Confirm">
</form>
<br>

<!-- Edit Form -->
<h2>Edit an Infection Type</h2>
<form method="post" action="infectionPage.php">
    Medicare Card Number: <input type="text" name="medicareCardNumber"><br>
    Date of Infection: <input type="date" name="date"><br><br>
    Change New Infection Type: <input type="text" name="newTypeToEdit"><br>
    <input type="submit" name="edit_infection" value="Confirm"><br><br>
</form>

<!-- q11, infected two weeks ago -->
<h2>Teachers Infected by COVID-19 in the Past Two Weeks (Q11)</h2>
<button onclick="window.location.href='infectedTwoWeeksAgo.php'">Infected Two Weeks Ago</button><br><br>

<!-- q15, High School + infected two weeks ago -->
<h2>High School Infection Numbers in the Past Two Weeks (Q15)</h2>
<button onclick="window.location.href='infectedTwoWeeksAgoHighSchool.php'">High School Infections</button><br<<br><br><br><br>


<!-- return to index.php -->
<a href="index.php">Go Back to Index Page</a>

</body>
</html>
