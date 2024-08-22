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

if(isset($_POST['deleteMedicareCardNumber'])) {
    $MedicareCardNumberToDelete = $_POST['deleteMedicareCardNumber'];
    
    // First, delete related records from the SchoolsAttendedByStudents table
    $deleteSchoolsQuery = "DELETE FROM SchoolsAttendedByStudents WHERE MedicareCardNumber = ?";
    $stmtSchools = mysqli_prepare($conn, $deleteSchoolsQuery);
    mysqli_stmt_bind_param($stmtSchools, 's', $MedicareCardNumberToDelete);
    mysqli_stmt_execute($stmtSchools);
    mysqli_stmt_close($stmtSchools);

    // Then, delete from the Student table
    $deleteStudentQuery = "DELETE FROM Student WHERE MedicareCardNumber = ?";
    $stmtStudent = mysqli_prepare($conn, $deleteStudentQuery);
    mysqli_stmt_bind_param($stmtStudent, 's', $MedicareCardNumberToDelete);
    mysqli_stmt_execute($stmtStudent);
    mysqli_stmt_close($stmtStudent);

    // Finally, delete from the Person table
    $deletePersonQuery = "DELETE FROM Person WHERE MedicareCardNumber = ?";
    $stmtPerson = mysqli_prepare($conn, $deletePersonQuery);
    mysqli_stmt_bind_param($stmtPerson, 's', $MedicareCardNumberToDelete);
    mysqli_stmt_execute($stmtPerson);

    if(mysqli_stmt_affected_rows($stmtPerson) > 0) {
        echo "Successfully deleted the person and associated records with Medicare Card Number: " . $MedicareCardNumberToDelete;
    } else {
        echo "No person found with the provided Medicare Card Number or an error occurred.";
    }

    mysqli_stmt_close($stmtPerson);
}

mysqli_close($conn);
?>
