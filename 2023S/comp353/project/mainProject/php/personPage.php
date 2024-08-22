<!DOCTYPE html>
<html>
<head>
    <title>Retrieve Person Information</title>
</head>
<body>
    <form action="fetchPerson.php" method="post">
        Enter MedicareCardNumber to find person: <input type="text" name="MedicareCardNumber">
        <input type="submit" value="Fetch Details">
    </form>

    <form action="fetchPerson.php" method="post">
    <input type="submit" name="fetch_all" value="Fetch All Persons">
</form>
</body>
</html>
