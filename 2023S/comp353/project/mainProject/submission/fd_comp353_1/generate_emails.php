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

// Get the current date and the date of the next Sunday
$currentDate = new DateTime();
$daysUntilNextSunday = 7 - $currentDate->format('w');
$nextSunday = clone $currentDate;
$nextSunday->modify("+$daysUntilNextSunday day");

// Calculate the start and end dates of the coming week
$startOfWeek = $nextSunday->format('Y-m-d');
$endOfWeek = $nextSunday->modify('+6 days')->format('Y-m-d');

// Query to retrieve facilities, employees, and their schedules for the coming week
$query = "
    SELECT
        F.Name AS FacilityName,
        F.Address AS FacilityAddress,
        P.FirstName AS EmployeeFirstName,
        P.LastName AS EmployeeLastName,
        P.EmailAddress AS EmployeeEmail,
        S.Date AS ScheduleDate,
        S.StartTime AS ScheduleStartTime,
        S.EndTime AS ScheduleEndTime
    FROM Facility F
    JOIN EmployeeSchedule S ON F.FacilityID = S.FacilityID AND F.MinistryID = S.MinistryID
    JOIN Person P ON S.MedicareCardNumber = P.MedicareCardNumber
    WHERE S.Date BETWEEN ? AND ?
";

$stmt = mysqli_prepare($conn, $query);
mysqli_stmt_bind_param($stmt, 'ss', $startOfWeek, $endOfWeek);
mysqli_stmt_execute($stmt);
$result = mysqli_stmt_get_result($stmt);

// Prepare the query to check for existing entries in EmailLog
$checkQuery = "SELECT COUNT(*) AS Count FROM EmailLog WHERE SenderFacilityName = ? AND ReceiverEmail = ? AND EmailSubject = ? AND EmailBody = ?";
$checkStmt = mysqli_prepare($conn, $checkQuery);

// Loop through the results and generate emails and log entries
while ($row = mysqli_fetch_assoc($result)) {
    $facilityName = $row['FacilityName'];
    $facilityAddress = $row['FacilityAddress'];
    $employeeFirstName = $row['EmployeeFirstName'];
    $employeeLastName = $row['EmployeeLastName'];
    $employeeEmail = $row['EmployeeEmail'];
    $scheduleDate = $row['ScheduleDate'];
    $scheduleStartTime = $row['ScheduleStartTime'];
    $scheduleEndTime = $row['ScheduleEndTime'];

    // Generate email content
    $subject = "{$facilityName} Schedule for {$scheduleDate}";
    $body = "Facility: {$facilityName}\nAddress: {$facilityAddress}\nEmployee: {$employeeFirstName} {$employeeLastName}\nEmail: {$employeeEmail}\n";
    $body .= "Schedule for {$scheduleDate}:\n";
    $body .= "Start Time: {$scheduleStartTime}\n";
    $body .= "End Time: {$scheduleEndTime}\n";

    // ... add more schedule details ...

    // Log email information
    $dateSent = date('Y-m-d H:i:s');
    $senderFacilityName = $facilityName;
    $receiverEmail = $employeeEmail;
    $emailSubject = $subject;
    $emailBody = substr($body, 0, 80);

    // Check if the entry already exists in EmailLog
    mysqli_stmt_bind_param($checkStmt, 'ssss', $senderFacilityName, $receiverEmail, $emailSubject, $emailBody);
    mysqli_stmt_execute($checkStmt);
    $checkResult = mysqli_stmt_get_result($checkStmt);
    $existingEntry = mysqli_fetch_assoc($checkResult);

    // If the entry doesn't exist, insert log entry
    if ($existingEntry['Count'] === 0) {
        $insertLogQuery = "INSERT INTO EmailLog (DateSent, SenderFacilityName, ReceiverEmail, EmailSubject, EmailBody)
                          VALUES (?, ?, ?, ?, ?)";
        $logStmt = mysqli_prepare($conn, $insertLogQuery);
        mysqli_stmt_bind_param($logStmt, 'sssss', $dateSent, $senderFacilityName, $receiverEmail, $emailSubject, $emailBody);
        mysqli_stmt_execute($logStmt);
        mysqli_stmt_close($logStmt);
    }
}

?>
