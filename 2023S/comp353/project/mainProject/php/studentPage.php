<!DOCTYPE html>
<html>
<head>
    <title>Retrieve Student Information</title>
</head>
<body>
    <form action="fetchStudent.php" method="post">
        Enter MedicareCardNumber to find student: <input type="text" name="MedicareCardNumber">
        <input type="submit" value="Fetch Details">
    </form>

    <form action="fetchStudent.php" method="post">
    <input type="submit" name="fetch_all" value="Fetch All Students">
</form>

<form action="deleteStudent.php" method="post">
    <label for="deleteMedicareCardNumber">Medicare Card Number to Delete:</label>
    <input type="text" id="deleteMedicareCardNumber" name="deleteMedicareCardNumber">
    <input type="submit" value="Delete Student">
</form>


</body>
</html>
