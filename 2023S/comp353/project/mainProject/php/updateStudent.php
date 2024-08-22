<?php
$host = 'fdc353.encs.concordia.ca';
$user = 'fdc353_1';
$pass = '353psswd';
$db = 'fdc353_1';

// Create connection
$conn = mysqli_connect($host, $user, $pass, $db);

// Check connection
if (!$conn) {
    die("Connection failed: " . mysqli_connect_error());
}
echo "Connected successfully<br>";


if (isset($_POST['MedicareCardNumber']) && isset($_POST['newFirstName']) && isset($_POST['newLastName'])) {
    $MedicareCardNumber = $_POST['MedicareCardNumber'];
    $newFirstName = $_POST['newFirstName'];
    $newLastName = $_POST['newLastName'];

    // Updating the Person table since it contains the names
    $updateQuery = "UPDATE Person SET FirstName = ?, LastName = ? WHERE MedicareCardNumber = ?";
    $stmt = mysqli_prepare($conn, $updateQuery);

    // Bind the newFirstName, newLastName and MedicareCardNumber to the ? placeholders
    mysqli_stmt_bind_param($stmt, 'sss', $newFirstName, $newLastName, $MedicareCardNumber);
    
    if (mysqli_stmt_execute($stmt)) {
        if (mysqli_stmt_affected_rows($stmt) > 0) {
            echo "Successfully updated the student's information.";
        } else {
            echo "No student found with the provided Medicare Card Number or no information was changed.";
        }
    } else {
        echo "An error occurred while updating the student's information.";
    }
    
    mysqli_stmt_close($stmt);
}

mysqli_close($conn);


?>