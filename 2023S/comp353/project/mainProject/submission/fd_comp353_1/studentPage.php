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



<h1>Update Student</h1>

<form action="updateStudent.php" method="post">
    <div>
        <label for="MedicareCardNumber">Medicare Card Number:</label>
        <input type="text" id="MedicareCardNumber" name="MedicareCardNumber" required>
    </div>
    <div>
        <label for="newFirstName">New First Name:</label>
        <input type="text" id="newFirstName" name="newFirstName" required>
    </div>
    <div>
        <label for="newLastName">New Last Name:</label>
        <input type="text" id="newLastName" name="newLastName" required>
    </div>
    <div>
        <input type="submit" value="Update">
    </div>
</form>

</body>
</html>








</body>
</html>
