<?php
$servername = "fdc353.encs.concordia.ca";
$username = "fdc353_1";
$password = "353psswd";
$dbname = "fdc353_1";

$conn = new mysqli($servername, $username, $password, $dbname);

if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $facilityID = $_POST["facilityID"];
    $ministryID = $_POST["ministryID"];
    $medicareCardNumber = $_POST["medicareCardNumber"];
    $date = $_POST["date"];
    $newStartTime = $_POST["newStartTime"];
    $newEndTime = $_POST["newEndTime"];

    $updateSql = "UPDATE EmployeeSchedule SET StartTime = ?, EndTime = ? WHERE FacilityID = ? AND MinistryID = ? AND MedicareCardNumber = ? AND Date = ?";
    $updateStmt = $conn->prepare($updateSql);
    $updateStmt->bind_param("ssiiis", $newStartTime, $newEndTime, $facilityID, $ministryID, $medicareCardNumber, $date);

    if ($updateStmt->execute()) {
        echo "Schedule updated successfully.";
    } else {
        echo "Error updating schedule: " . $updateStmt->error;
    }

    $updateStmt->close();
}

$conn->close();
?>
