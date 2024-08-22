<?php
// Assuming you have a database connection established
$servername = "fdc353.encs.concordia.ca";
$username = "fdc353_1";
$password = "353psswd";
$dbname = "fdc353_1";

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);

// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}


if (isset($_POST['deleteSubmit'])) {
    $medicareCardNumberToDelete = $_POST['MedicareCardNumberToDelete'];

    // Delete records from related tables
    $deleteEmployeeSchedule = "DELETE FROM EmployeeSchedule WHERE MedicareCardNumber = ?";
    $deleteWorkplaceForEachEmployee = "DELETE FROM WorkplaceForEachEmployee WHERE MedicareCardNumber = ?";
    $deleteEmployee = "DELETE FROM Employee WHERE MedicareCardNumber = ?";
    $deletePerson = "DELETE FROM Person WHERE MedicareCardNumber = ?";
    $deleteInfection ="DELETE FROM Infection WHERE MedicareCardNumber = ?";
    $deleteVaccination ="DELETE FROM Vaccination WHERE MedicareCardNumber = ?";

    // Prepare and bind the delete queries
    $stmt5 = $conn->prepare($deleteInfection);
    $stmt5->bind_param("i", $medicareCardNumberToDelete);
    $stmt5->execute();

    $stmt6 =$conn->prepare($deleteVaccination);
    $stmt6->bind_param("i", $medicareCardNumberToDelete);
    $stmt6->execute();
    
    $stmt1 = $conn->prepare($deleteEmployeeSchedule);
    $stmt1->bind_param("i", $medicareCardNumberToDelete);
    $stmt1->execute();

    $stmt2 = $conn->prepare($deleteWorkplaceForEachEmployee);
    $stmt2->bind_param("i", $medicareCardNumberToDelete);
    $stmt2->execute();

    $stmt3 = $conn->prepare($deleteEmployee);
    $stmt3->bind_param("i", $medicareCardNumberToDelete);
    $stmt3->execute();

    $stmt4 = $conn->prepare($deletePerson);
    $stmt4->bind_param("i", $medicareCardNumberToDelete);
    $stmt4->execute();



    // Check if any of the queries resulted in an error
    if ($conn->error) {
        echo "Error deleting records: " . $conn->error;
    } else {
        echo "Employee and related records deleted successfully.";
    }

    // Close prepared statements
    $stmt1->close();
    $stmt2->close();
    $stmt3->close();
    $stmt4->close();
    $stmt5->close();
    $stmt6->close();
}

// Close the database connection
$conn->close();
?>

