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
    $deleteFacilityID = $_POST["delete_facility_id"];
    $deleteMinistryID = $_POST["delete_ministry_id"];
    $deleteMedicareCardNumber = $_POST["delete_medicare_card_number"];
    $deleteDate = $_POST["delete_date"];

    $deleteSql = "DELETE FROM EmployeeSchedule WHERE FacilityID = ? AND MinistryID = ? AND MedicareCardNumber = ? AND Date = ?";
    $deleteStmt = $conn->prepare($deleteSql);
    $deleteStmt->bind_param("iiss", $deleteFacilityID, $deleteMinistryID, $deleteMedicareCardNumber, $deleteDate);

    if ($deleteStmt->execute()) {
        echo "Schedule deleted successfully.";
    } else {
        echo "Error deleting schedule: " . $deleteStmt->error;
    }

    $deleteStmt->close();
}

$conn->close();
?>
