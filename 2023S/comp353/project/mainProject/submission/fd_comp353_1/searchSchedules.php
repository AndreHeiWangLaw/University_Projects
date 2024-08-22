<?php
$host = 'fdc353.encs.concordia.ca';
$user = 'fdc353_1';
$pass = '353psswd';
$db = 'fdc353_1';

$conn = new mysqli($host, $user, $pass, $db);

if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

if ($_SERVER["REQUEST_METHOD"] == "GET") {
    $employeeMedicareCardNumber = $_GET["medicareCardNumber"];
    $startDate = $_GET["startDate"];
    $endDate = $_GET["endDate"];

    $sql = "SELECT F.Name AS FacilityName, ES.Date, ES.StartTime, ES.EndTime
            FROM EmployeeSchedule AS ES
            JOIN Facility AS F ON ES.FacilityID = F.FacilityID AND ES.MinistryID = F.MinistryID
            WHERE ES.MedicareCardNumber = ? AND ES.Date BETWEEN ? AND ?
            ORDER BY F.Name, ES.Date, ES.StartTime";

    $stmt = $conn->prepare($sql);
    $stmt->bind_param("iss", $employeeMedicareCardNumber, $startDate, $endDate);
    $stmt->execute();
    $result = $stmt->get_result();

    if ($result->num_rows > 0) {
        echo "<h2>Schedules for Employee with Medicare Card Number: $employeeMedicareCardNumber</h2>";
        echo "<table>";
        echo "<tr><th>Facility Name</th><th>Date</th><th>Start Time</th><th>End Time</th></tr>";
        while ($row = $result->fetch_assoc()) {
            echo "<tr>";
            echo "<td>" . $row["FacilityName"] . "</td>";
            echo "<td>" . $row["Date"] . "</td>";
            echo "<td>" . $row["StartTime"] . "</td>";
            echo "<td>" . $row["EndTime"] . "</td>";
            echo "</tr>";
        }
        echo "</table>";
    } else {
        echo "No schedules found for the specified employee and time frame.";
    }

    $stmt->close();
}

$conn->close();
?>
