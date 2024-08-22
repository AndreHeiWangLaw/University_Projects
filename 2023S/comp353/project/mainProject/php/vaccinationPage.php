<!DOCTYPE html>
<html>
<head>
    <title>Vaccination Management</title>
</head>
<body>
<h1>Vaccination Management</h1>

<!-- Create Vaccination Record -->
<h2>Create Vaccination Record</h2>
<a href="createVaccination.php">Create New Vaccination Record</a><br><br>

<!-- Display Vaccination Records -->
<h2>Display Vaccination Records</h2>
<a href="fetchVaccinations.php">Display All Vaccination Records</a><br><br>

<!-- Edit Vaccination Record -->
<h2>Edit Vaccination Record</h2>
<p>Enter the Medicare Card Number and Date of the Vaccination to edit:</p>
<form action="editVaccination.php" method="GET">
    Medicare Card Number: <input type="number" name="medicareCardNumber"><br>
    Date of Vaccination: <input type="date" name="date"><br>
    <input type="submit" value="Edit Vaccination Record">
</form><br>

<!-- Delete Vaccination Record -->
<h2>Delete Vaccination Record</h2>
<p>Enter the Medicare Card Number and Date of the Vaccination to delete:</p>
<form action="deleteVaccination.php" method="GET">
    Medicare Card Number: <input type="number" name="medicareCardNumber"><br>
    Date of Vaccination: <input type="date" name="date"><br>
    <input type="submit" value="Delete Vaccination Record">
</form><br>
</body>
</html>
