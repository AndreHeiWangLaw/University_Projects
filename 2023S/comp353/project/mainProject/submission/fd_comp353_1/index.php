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
<button onclick="window.location.href='facilityPage.php'">Facility (1,13,14)</button>

<button onclick="window.location.href='personPage.php'">Person</button>
<button onclick="window.location.href='studentPage.php'">Student (2)</button>
<button onclick="window.location.href='employeePage.php'">Employee (3)</button>
<button onclick="window.location.href='vacPage.php'">Vaccination (4)</button>
<button onclick="window.location.href='infectionPage.php'">Infection (5,11,15)</button>
<button onclick="window.location.href='6.php'">Register/Modify registration/Cancel registration for a student in a school 6</button>
<button onclick="window.location.href='schedule.html'">Schedule (7)</button>
<button onclick="window.location.href='emailLog.php'">emailLog (12,20)</button>
<form action="8.php" method="post">
        <button type="submit" name="executeSql">Get details of all the facilities in the system. 8</button>
</form>

<button onclick="window.location.href='9.html'">Details of all employees working in a specific facility (9)</button>
<button onclick="window.location.href='10.html'">Get Daily Schedule of the employee (10)</button>

    <form action="16.php" method="post">
        <button type="submit" name="executeSql">16</button>
    </form>

</body>
</html>




