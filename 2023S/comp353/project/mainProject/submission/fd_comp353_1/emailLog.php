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

    $facilityName = $_POST['facilityName'];

    // Prepare and execute the query
    $query = "SELECT * FROM EmailLog WHERE SenderFacilityName = ? ORDER BY DateSent ASC";
    $stmt = mysqli_prepare($conn, $query);
    mysqli_stmt_bind_param($stmt, "s", $facilityName);
    mysqli_stmt_execute($stmt);
    $result = mysqli_stmt_get_result($stmt);

    // Display the results
    if (mysqli_num_rows($result) > 0) {
        echo "<table border='1'>";
        echo "<tr><th>Log ID</th><th>Date Sent</th><th>Sender Facility</th><th>Receiver Email</th><th>Subject</th><th>Body</th></tr>";

        while ($row = mysqli_fetch_assoc($result)) {
            echo "<tr>";
            echo "<td>" . $row["LogID"] . "</td>";
            echo "<td>" . $row["DateSent"] . "</td>";
            echo "<td>" . $row["SenderFacilityName"] . "</td>";
            echo "<td>" . $row["ReceiverEmail"] . "</td>";
            echo "<td>" . $row["EmailSubject"] . "</td>";
            echo "<td>" . $row["EmailBody"] . "</td>";
            echo "</tr>";
        }

        echo "</table>";
    } else {
        echo "No results found.";
    };

    ?>


<!DOCTYPE html>
<html>

<!-- Create -->
<h2>Generate Email Log</h2>
<form method="post" action="generate_emails.php">
        <input type="submit" name="generate_emails" value="Generate Emails and Populate Log">
    </form>
<br>

<!-- 12 -->
<h2>12 - Generate emailLog for a facility</h2>
<form method="post" action="<?php echo $_SERVER["PHP_SELF"]; ?>">
        Facility ID: <input type="text" name="facilityName">
        <input type="submit" value="12">
    </form>
</html>