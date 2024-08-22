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
    $startTime = $_POST["startTime"];
    $endTime = $_POST["endTime"];

    $sql = "INSERT INTO EmployeeSchedule (FacilityID, MinistryID, MedicareCardNumber, Date, StartTime, EndTime) VALUES (?, ?, ?, ?, ?, ?)";
    $stmt = $conn->prepare($sql);
    $stmt->bind_param("iissss", $facilityID, $ministryID, $medicareCardNumber, $date, $startTime, $endTime);

    if ($stmt->execute()) {
        echo "Schedule assigned successfully.";
    } else {
        echo "Error assigning schedule: " . $stmt->error;
    }

    $stmt->close();
}

$conn->close();
?>
