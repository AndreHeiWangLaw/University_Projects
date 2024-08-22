<!DOCTYPE html>
<html>
<head>
    <title>Retrieve Employee Information</title>
</head>
<body>
    <form action="fetchEmployee.php" method="post">
        Enter SSN: <input type="text" name="MedicareCardNumber">
        <input type="submit" value="Fetch Details">
        <br>
       <input type="submit" name = "fetch_all" value = "give me all the details">
       <br>
</form>

<form action="deleteEmployee.php" method="post">
    Enter Medicare Number to Delete: <input type="text" name="MedicareCardNumberToDelete">
    <input type="submit" name="deleteSubmit" value="Delete">
</form>

</body>
</html>
