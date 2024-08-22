<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Database Interface Index</title>
    <style>
        /* Simple styling to space out the buttons and center them */
        body {
            display: flex;
            flex-direction: column;
            align-items: center;
            gap: 10px;
            padding-top: 50px;
            font-family: Arial, sans-serif;
        }
        button {
            padding: 10px 20px;
            cursor: pointer;
        }
    </style>
</head>
<body>

<button onclick="window.location.href='personPage.php'">Person</button>
<button onclick="window.location.href='studentPage.php'">Student</button>
<button onclick="window.location.href='employeePage.html'">Employee</button>
<button onclick="window.location.href='schoolsAttendedByStudentsPage.html'">Schools Attended By Students</button>
<button onclick="window.location.href='WorkplaceForEachEmployee.html'">Workplace For Each Employee</button>
<button onclick="window.location.href='infections.html'">Infection</button>
<button onclick="window.location.href='vacPage.php'">Vaccination</button>
<button onclick="window.location.href='ministryPage.html'">Ministry</button>
<button onclick="window.location.href='facilityPage.html'">Facility</button>
<button onclick="window.location.href='educationalFacilityPage.html'">Educational</button>
<button onclick="window.location.href='managementFacilityPage.html'">Management</button>
<button onclick="window.location.href='employeeSchedulePage.html'">Employee Schedule</button>

</body>
</html>


<?php
include 'config.php';

// Write your SQL query
$sql = "SELECT * FROM person"; // Assuming your table is named "person"
$result = $conn->query($sql);

$people = [];
if ($result->num_rows > 0) {
    // Fetch data into an associative array
    while($row = $result->fetch_assoc()) {
        $people[] = $row;
    }
}

// Close the connection
$conn->close();
?>

