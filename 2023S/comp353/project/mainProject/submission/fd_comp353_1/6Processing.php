<?php
$host = 'localhost';
$user = 'root';
$db = 'fdc353_1';

$conn = mysqli_connect($host, $user, '', $db);

if (!$conn) {
    die("Connection failed: " . mysqli_connect_error());
}

$operation = $_POST['operation'];
$MedicareCardNumber = $_POST['MedicareCardNumber'];
$FacilityID = $_POST['FacilityID'];
$MinistryID = $_POST['MinistryID'];
$Level = $_POST['Level'];

if ($operation == "register") {
    $sqlStudent = "INSERT INTO Student (MedicareCardNumber, FacilityID, MinistryID, Level) VALUES ('$MedicareCardNumber', '$FacilityID', '$MinistryID', '$Level')";
    $sqlSchool = "INSERT INTO SchoolsAttendedByStudents (MedicareCardNumber, FacilityID, MinistryID, Level, StartDate) VALUES ('$MedicareCardNumber', '$FacilityID', '$MinistryID', '$Level', NOW())"; // Assuming StartDate is current date
} elseif ($operation == "modify") {
    $sqlStudent = "UPDATE Student SET FacilityID='$FacilityID', MinistryID='$MinistryID', Level='$Level' WHERE MedicareCardNumber='$MedicareCardNumber'";
    $sqlSchool = "UPDATE SchoolsAttendedByStudents SET FacilityID='$FacilityID', MinistryID='$MinistryID', Level='$Level', StartDate=NOW() WHERE MedicareCardNumber='$MedicareCardNumber'"; // Assuming you want to update StartDate
} elseif ($operation == "cancel") {
    $sqlStudent = "DELETE FROM Student WHERE MedicareCardNumber='$MedicareCardNumber'";
    $sqlSchool = "DELETE FROM SchoolsAttendedByStudents WHERE MedicareCardNumber='$MedicareCardNumber'";
}

if ($conn->query($sqlStudent) === TRUE && $conn->query($sqlSchool) === TRUE) {
    echo "Operation success!";
} else {
    echo "Error: " . $sqlStudent . "<br>" . $conn->error;
    echo "Error: " . $sqlSchool . "<br>" . $conn->error;
}

$conn->close();
?>
