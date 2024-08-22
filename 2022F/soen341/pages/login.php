<?php
session_start();
?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8"/>
    <meta http-equiv="X-UA-Compatible" content="IE=edge"/>
    <meta name="viewport" content="width=device-width, initial-scale=1.0"/>
    <title>Sign In</title>
</head>

<link
        href="https://fonts.googleapis.com/css?family=Urbanist"
        rel="stylesheet"
/>

<style>
    .log-in-button {
        width: 330px;
        height: 45px;
        border: black;
        background: rgb(221, 221, 221);
        -webkit-box-shadow: 5px 5px 10px -5px #595959;
        box-shadow: 5px 5px 10px -5px #595959;
        color: black;
        font-size: 16.5px;
        margin: 10px 0 0 0;
        border-radius: 20px;
    }

    body {
        background-color: white;
    }

    h1 {
        text-align: center;
        font-size: 50px;
        color: #157347;
        font-family: "Urbanist";
    }

    h4 {
        text-align: center;
        font-size: 14px;
        color: red;
        font-family: "Urbanist";
    }

    form {
        margin: 20px auto;
        width: 320px;
        color: black;
    }

    input {
        padding: 10px;
        font-size: inherit;
    }

    input[type="text"] {
        display: block;
        margin-bottom: 25px;
        width: 95%;
        border: 2px solid black;
        background: #dddddd;
    }

    input[type="email"] {
        display: block;
        margin-bottom: 25px;
        width: 95%;
        border: 2px solid #ebebeb !important;
        background: #fbfbfb !important;
        border-radius: 12px !important;
    }

    input[type="password"] {
        display: block;
        margin-bottom: 25px;
        width: 95%;
        border: 2px solid #ebebeb !important;
        background: #fbfbfb !important;
        border-radius: 12px !important;
    }

    input[type="button"] {
        width: 330px;
        height: 45px;
        border: black;
        background: rgb(221, 221, 221);
        -webkit-box-shadow: 5px 5px 10px -5px #595959;
        box-shadow: 5px 5px 10px -5px #595959;
        color: black;
        border-radius: 20px;
        margin: 10px 0 0 0;
    }

    button {
        width: 330px;
        height: 45px;
        border: black;
        background: rgb(221, 221, 221);
        -webkit-box-shadow: 5px 5px 10px -5px #595959;
        box-shadow: 5px 5px 10px -5px #595959;
        color: black;
        font-size: 16.5px;
        margin: 10px 0 0 0;
        border-radius: 20px;
    }

    input:focus {
        background: white;
    }

    .submissionform {
        max-width: 728px;
        margin: 2rem auto;
        border: 3px solid;
        border-radius: 20px;
        -webkit-box-shadow: 5px 5px 10px -5px #595959;
        box-shadow: 5px 5px 10px -5px #595959;
        padding: 3rem;
        display: block;
        background-color: rgb(240, 240, 240);
        border-color: rgb(240, 240, 240);
    }
</style>

<body>
<h1>Login To Your Account</h1>


<form class="submissionform" method="post" id="form">
    <input name="email" style="font-family: Urbanist" type="Email" placeholder="Email" id="email"/>
    <input name="password" style="font-family: Urbanist" type="Password" placeholder="Password" id="password"/>
    <input class="log-in-button" name="login" style="font-family: Urbanist" type="submit" id="submitButton"
           value="Log In">
    <?php
    require("../php/util/user_reader.php");

    if (isset($_POST['email']) and isset($_POST['password'])) {
        $email = $_POST['email'];
        $password = $_POST['password'];

        if ($email != '' && $password != '')
            if (authenticate($email, $password)) {
                $_SESSION['username'] = get_user_property($email, "firstName");
                $_SESSION['email'] = $email;
                $_SESSION['role'] = get_user_property($email, "role");
                $_SESSION['id'] = get_user_property($email,"id");
                $username = strtolower($_SESSION['username']);
                if (strcasecmp($_SESSION['role'], 'supervisor') == 0)
                    echo "<script type='text/javascript'>document.location = \"backstore/supervisor/purchases.php?username=\" + \"{$username}\" ;</script>";
                else if (strcasecmp($_SESSION['role'], 'supplier') == 0) {
                    echo "<script type='text/javascript'>document.location = \"backstore/supplier/inventory.php?username=\" + \"{$username}\" ;</script>";
                }else if (strcasecmp($_SESSION['role'], 'customer') == 0){
                    echo "<script type='text/javascript'>document.location = \"frontstore/shop.php?username=\" + \"{$username}\" ;</script>";
                }
            } else {
                echo "<h4>Wrong username or wrong password.</h4>";
            }
    }
    ?>
</form>

<script src="login.js" rel="script"></script>
</body>
</html>