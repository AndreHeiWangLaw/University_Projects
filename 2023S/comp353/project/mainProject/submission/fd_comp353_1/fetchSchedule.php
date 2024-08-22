<?php
$servername = "fdc353.encs.concordia.ca";
$username = "fdc353_1";
$password = "353psswd";
$dbname = "fdc353_1";

$conn = new mysqli($servername, $username, $password, $dbname);

if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

$sql = "SELECT * FROM EmployeeSchedule";
$result = $conn->query($sql);

if ($result->num_rows > 0) {
    echo "<table>";
    echo "<tr><th>Facility ID</th><th>Ministry ID</th><th>Medicare Card Number</th><th>Date</th><th>Start Time</th><th>End Time</th></tr>";
    while ($row = $result->fetch_assoc()) {
        echo "<tr>";
        echo "<td>" . $row["FacilityID"] . "</td>";
        echo "<td>" . $row["MinistryID"] . "</td>";
        echo "<td>" . $row["MedicareCardNumber"] . "</td>";
        echo "<td>" . $row["Date"] . "</td>";
        echo "<td>" . $row["StartTime"] . "</td>";
        echo "<td>" . $row["EndTime"] . "</td>";
        echo "</tr>";
    }
    echo "</table>";
} else {
    echo "No schedules found.";
}

$conn->close();
?>
