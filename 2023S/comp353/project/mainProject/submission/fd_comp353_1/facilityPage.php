<?php

$host = 'fdc353.encs.concordia.ca';
$user = 'fdc353_1';
$pass = '353psswd';
$db = 'fdc353_1'; // replace 'your_database_name' with your actual database name

// Create connection
$conn = mysqli_connect($host, $user, $pass, $db);

// Check connection
if (!$conn) {
  die("Connection failed: " . mysqli_connect_error());
}
echo "Connected successfully<br>";

// Create
if (isset($_POST['add_facility'])) {
    $FacilityID = $_POST['FacilityID'];
    $MinistryID = $_POST['MinistryID'];
    $Name = $_POST['Name'];
    $Address = $_POST['Address'];
    $City = $_POST['City'];
    $Province = $_POST['Province'];
    $CapOnEmployees = $_POST['CapOnEmployees'];
    $PostalCode = $_POST['PostalCode'];
    $Phone = $_POST['Phone'];
    $WebAddress = $_POST['WebAddress'];
    $Type = $_POST['Type'];
    $Principle = $_POST['Principle'];
    $PresidentID = $_POST['PresidentID'];
    $HeadOffice = isset($_POST['HeadOffice']) ? 1 : 0;

    // First, insert into the Facility table
    $insert_query_facility = "INSERT INTO Facility(FacilityID, MinistryID, Name, Address, City, Province, CapOnEmployees, PostalCode, Phone, WebAddress, Type)
                              VALUES ('$FacilityID', '$MinistryID', '$Name', '$Address', '$City', '$Province', '$CapOnEmployees', '$PostalCode', '$Phone', '$WebAddress', '$Type')";

    if ($conn->query($insert_query_facility)) {
        // Facility record inserted successfully

        if ($Type == "Education") {
            // Insert into Educational table
            $lastWords = implode(' ', array_slice(explode(' ', $Name), -2));
            $insert_query_educational = "INSERT INTO Educational(FacilityID, MinistryID, Principle, Type)
                                         VALUES ('$FacilityID', '$MinistryID', '$Principle', '$lastWords')";
            if ($conn->query($insert_query_educational)) {
                // Educational record inserted successfully
            } else {
                // Error inserting educational record
            }
        } else {
            // Insert into Management table
            $lastWord = end(explode(' ', $Name));
            $insert_query_management = "INSERT INTO Management(FacilityID, MinistryID, PresidentID, HeadOffice)
                                        VALUES ('$FacilityID', '$MinistryID', '$PresidentID', '$HeadOffice')";
            if ($conn->query($insert_query_management)) {
                // Management record inserted successfully
            } else {
                // Error inserting management record
            }
        }
    } else {
        // Error inserting facility record
    }
}




// Edit Facility

if (isset($_POST['edit_facility'])) {
    $FacilityID = $_POST['FacilityID'];
    $MinistryID = $_POST['MinistryID'];
    
    // Fetch existing facility details
    $select_query = "SELECT * FROM Facility WHERE FacilityID='$FacilityID' AND MinistryID='$MinistryID'";
    $result = $conn->query($select_query);
    $existing_data = $result->fetch_assoc();
    
    // Get new values from form, or use existing values if empty
    $Name = $_POST['Name'] ?: $existing_data['Name'];
    $Address = $_POST['Address'] ?: $existing_data['Address'];
    $City = $_POST['City'] ?: $existing_data['City'];
    $Province = $_POST['Province'] ?: $existing_data['Province'];
    $CapOnEmployees = $_POST['CapOnEmployees'] ?: $existing_data['CapOnEmployees'];
    $PostalCode = $_POST['PostalCode'] ?: $existing_data['PostalCode'];
    $Phone = $_POST['Phone'] ?: $existing_data['Phone'];
    $WebAddress = $_POST['WebAddress'] ?: $existing_data['WebAddress'];
    $Type = $_POST['Type'] ?: $existing_data['Type'];
    $Principle = $_POST['Principle'] ?: $existing_data['Principle'];
    $PresidentID = $_POST['PresidentID'] ?: $existing_data['PresidentID'];
    $HeadOffice = isset($_POST['HeadOffice']) ? 1 : 0;

    // Update the Facility table
    $update_query = "UPDATE Facility
                     SET Name='$Name', Address='$Address', City='$City', Province='$Province', CapOnEmployees='$CapOnEmployees',
                         PostalCode='$PostalCode', Phone='$Phone', WebAddress='$WebAddress', Type='$Type'
                     WHERE FacilityID='$FacilityID' AND MinistryID='$MinistryID'";
    
    if ($conn->query($update_query)) {
        // Facility record updated successfully

        // Delete old records if Type is changed
        if ($Type == "Education") {
            $delete_query_management = "DELETE FROM Management WHERE FacilityID='$FacilityID' AND MinistryID='$MinistryID'";
            $conn->query($delete_query_management);

            // Insert into Educational table
            $lastWords = implode(' ', array_slice(explode(' ', $Name), -2));
            $insert_query_educational = "INSERT INTO Educational(FacilityID, MinistryID, Principle, Type)
                                         VALUES ('$FacilityID', '$MinistryID', '$Principle', '$lastWords')";
            $conn->query($insert_query_educational);
        } else {
            $delete_query_educational = "DELETE FROM Educational WHERE FacilityID='$FacilityID' AND MinistryID='$MinistryID'";
            $conn->query($delete_query_educational);

            // Insert or update Management record
            $insert_query_management = "INSERT INTO Management(FacilityID, MinistryID, PresidentID, HeadOffice)
                                        VALUES ('$FacilityID', '$MinistryID', '$PresidentID', '$HeadOffice')
                                        ON DUPLICATE KEY UPDATE PresidentID='$PresidentID', HeadOffice='$HeadOffice'";
            $conn->query($insert_query_management);
        }
    } else {
        // Error updating facility record
    }
}


// Delete Facility
if (isset($_POST['delete_facility'])) {
    $FacilityID = $_POST['FacilityID'];
    $MinistryID = $_POST['MinistryID'];

    // Fetch facility type
    $type_query = "SELECT Type FROM Facility WHERE FacilityID='$FacilityID' AND MinistryID='$MinistryID'";
    $result = $conn->query($type_query);
    
    if ($result->num_rows > 0) {
        $row = $result->fetch_assoc();
        $facilityType = $row['Type'];

        // Disable foreign key checks
        $conn->query("SET FOREIGN_KEY_CHECKS=0");

        // Delete from the corresponding table (Educational or Management)
        if ($facilityType == "Education") {
            $delete_query_educational = "DELETE FROM Educational WHERE FacilityID='$FacilityID' AND MinistryID='$MinistryID'";
            $conn->query($delete_query_educational);
        } else {
            $delete_query_management = "DELETE FROM Management WHERE FacilityID='$FacilityID' AND MinistryID='$MinistryID'";
            $conn->query($delete_query_management);
        }

        // Delete from the Facility table
        $delete_query_facility = "DELETE FROM Facility WHERE FacilityID='$FacilityID' AND MinistryID='$MinistryID'";
        if ($conn->query($delete_query_facility)) {
            // Facility record deleted successfully
        } else {
            // Error deleting facility record
        }

        // Re-enable foreign key checks
        $conn->query("SET FOREIGN_KEY_CHECKS=1");
    } else {
        // Facility not found
    }
}


// Query 13
if(isset($_POST['query_13'])) {
    $facilityID = $_POST['FacilityID'];

    // Get list of teachers from the last two weeks
    $two_weeks_ago = date('Y-m-d', strtotime('-2 weeks'));
    $teachers_query = "SELECT DISTINCT Person.FirstName, Person.LastName, Employee.Role 
                       FROM Person
                       INNER JOIN Employee ON Person.MedicareCardNumber = Employee.MedicareCardNumber
                       INNER JOIN EmployeeSchedule ON Employee.MedicareCardNumber = EmployeeSchedule.MedicareCardNumber
                       WHERE EmployeeSchedule.FacilityID = ? AND EmployeeSchedule.Date >= ?
                       ORDER BY Employee.Role ASC, Person.FirstName ASC";
    
    $stmt = mysqli_prepare($conn, $teachers_query);

    mysqli_stmt_bind_param($stmt, 'is', $facilityID, $two_weeks_ago);
    mysqli_stmt_execute($stmt);

    $result = mysqli_stmt_get_result($stmt);

    if (mysqli_num_rows($result) > 0) {
        echo "<h2>List of Teachers at Facility $facilityID in the Last Two Weeks</h2>";
        echo "<table border='1'>";
        echo "<tr><th>First Name</th><th>Last Name</th><th>Role</th></tr>";

        while ($row = mysqli_fetch_assoc($result)) {
            echo "<tr>";
            echo "<td>" . $row['FirstName'] . "</td>";
            echo "<td>" . $row['LastName'] . "</td>";
            echo "<td>" . $row['Role'] . "</td>";
            echo "</tr>";
        }
        echo "</table>";
    } else {
        echo "No records found.";
    }

    mysqli_stmt_close($stmt);
}

// Query 14
if(isset($_POST['query_total_hours'])) {
    $facilityID = $_POST['FacilityID'];
    $startDate = $_POST['StartDate'];
    $endDate = $_POST['EndDate'];

    // Get total hours scheduled for every teacher during the specific period
    $total_hours_query = "SELECT Person.FirstName, Person.LastName, EmployeeSchedule.MedicareCardNumber, 
                          SUM(TIME_TO_SEC(TIMEDIFF(EndTime, StartTime))) / 3600 AS TotalHours
                          FROM Person
                          INNER JOIN Employee ON Person.MedicareCardNumber = Employee.MedicareCardNumber
                          INNER JOIN EmployeeSchedule ON Employee.MedicareCardNumber = EmployeeSchedule.MedicareCardNumber
                          WHERE EmployeeSchedule.FacilityID = ? AND EmployeeSchedule.Date BETWEEN ? AND ?
                          GROUP BY Person.FirstName, Person.LastName, EmployeeSchedule.MedicareCardNumber
                          ORDER BY Person.FirstName ASC, Person.LastName ASC";
    
    $stmt = mysqli_prepare($conn, $total_hours_query);

    mysqli_stmt_bind_param($stmt, 'iss', $facilityID, $startDate, $endDate);
    mysqli_stmt_execute($stmt);

    $result = mysqli_stmt_get_result($stmt);

    if (mysqli_num_rows($result) > 0) {
        echo "<h2>Total Hours Scheduled for Teachers at Facility $facilityID</h2>";
        echo "<table border='1'>";
        echo "<tr><th>First Name</th><th>Last Name</th><th>MedicareCardNumber</th><th>Total Hours</th></tr>";

        while ($row = mysqli_fetch_assoc($result)) {
            echo "<tr>";
            echo "<td>" . $row['FirstName'] . "</td>";
            echo "<td>" . $row['LastName'] . "</td>";
            echo "<td>" . $row['MedicareCardNumber'] . "</td>";
            echo "<td>" . $row['TotalHours'] . " hours</td>";
            echo "</tr>";
        }
        echo "</table>";
    } else {
        echo "No records found.";
    }

    mysqli_stmt_close($stmt);
}

?>

<!DOCTYPE html>
<html>
<head>
    <title>Retrieve facility Information</title>
</head>

<h2>Display a Facility (Enter the name OR the facilityID AND ministryID)</h2>
<body>
<form method="post" action="fetchFacility.php">
        <label for="FacilityName">Facility Name:</label>
        <input type="text" name="FacilityName"><br>
        
        <label for="FacilityID">Facility ID:</label>
        <input type="text" name="FacilityID"><br>
        
        <label for="MinistryID">Ministry ID:</label>
        <input type="text" name="MinistryID"><br>
        
        <input type="submit" name="display_facility" value="Display Facility">
    </form>

    <form action="fetchFacility.php" method="post">
    <input type="submit" name="fetch_all" value="Fetch All Facilities">
</form>

<!-- Delete -->
<h2>Delete a Facility (input both values to delete)</h2>
<form method="post">
    FacilityID: <input type="text" name="FacilityID"><br>
    MinistryID: <input type="text" name="MinistryID"><br>
    <input type="submit" name="delete_facility" value="Confirm">
</form>
<br>

<!-- Create -->
<h2>Create a Facility</h2>
<form method="post">
    FacilityID: <input type="text" name="FacilityID"><br>
    MinistryID: <input type="text" name="MinistryID"><br>
    Name: <input type="text" name="Name"><br>
    Address: <input type="text" name="Address"><br>
    City: <input type="text" name="City"><br>
    Province: <input type="text" name="Province"><br>
    CapOnEmployees: <input type="text" name="CapOnEmployees"><br>
    PostalCode: <input type="text" name="PostalCode"><br>
    Phone: <input type="text" name="Phone"><br>
    WebAddress: <input type="text" name="WebAddress"><br>
    Type: <input type="text" name="Type"><br>
    Principle: <input type="text" name="Principle"><br>
    PresidentID: <input type="text" name="PresidentID"><br>
    HeadOffice: <input type="text" name="HeadOffice"><br>

    <input type="submit" name="add_facility" value="Confirm">
</form>
<br>

<!-- Edit -->
<h2>Edit a Facility (input the both IDs for the facility that you want to modify)</h2>
<form method="post">
    
    FacilityID: <input type="text" name="FacilityID"><br>
    MinistryID: <input type="text" name="MinistryID"><br>
    Name: <input type="text" name="Name"><br>
    Address: <input type="text" name="Address"><br>
    City: <input type="text" name="City"><br>
    Province: <input type="text" name="Province"><br>
    CapOnEmployees: <input type="text" name="CapOnEmployees"><br>
    PostalCode: <input type="text" name="PostalCode"><br>
    Phone: <input type="text" name="Phone"><br>
    WebAddress: <input type="text" name="WebAddress"><br>
    Type: <input type="text" name="Type"><br>
    Principle: <input type="text" name="Principle"><br>
    PresidentID: <input type="text" name="PresidentID"><br>
    HeadOffice: <input type="text" name="HeadOffice"><br>

    <input type="submit" name="edit_facility" value="Confirm">
</form>
<br>

<!-- 13 -->
<h2>Query 13: List of Teachers in the Last Two Weeks</h2>
<form method="post">
    Facility ID: <input type="text" name="FacilityID"><br>
    <input type="submit" name="query_13" value="13">
</form>
<br>

<!-- 14 -->
<h2>Query 14: Total Hours Scheduled for Teachers</h2>
    <form method="post">
        Facility ID: <input type="text" name="FacilityID"><br>
        Start Date: <input type="date" name="StartDate"><br>
        End Date: <input type="date" name="EndDate"><br>
        <input type="submit" name="query_total_hours" value="Submit">
    </form>

</body>
</html>
