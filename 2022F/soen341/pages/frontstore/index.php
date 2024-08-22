<?php
session_start();
?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0"/>
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css">
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.0.2/dist/js/bootstrap.bundle.min.js"
            integrity="sha384-MrcW6ZMFYlzcLA8Nl+NtUVF0sA7MsXsP1UyJoMp4YLEuNSfAP+JcXn/tWtIaxVXM"
            crossorigin="anonymous"></script>
    <style>
        @import url('https://fonts.googleapis.com/css2?family=Urbanist:wght@200&display=swap');
    </style>
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.1.3/dist/css/bootstrap.min.css" rel="stylesheet"
          integrity="sha384-1BmE4kWBq78iYhFldvKuhfTAU6auU8tT94WrHftjDbrCEXSU1oBoqyl2QvZ6jIW3"
          crossorigin="anonymous">
    <link rel="stylesheet" href="../css/store/styles_backstore.css"/>
    <title>Inventory</title>
</head>
<body>
<main>
    <div class="container-md fa-babyblue">
        <div class="row transparent">
            <div class="col-md-3">
                <h1 style="color: black;">Welcome Page</h1>
            </div>
            <div class="col-md-2">
                <a href="../login.php" class="btn btn-dark btn-lg" role="button"><h3>Login</h3></a>
            </div>
            <div class="col-md-2">
                <a href="../sign_up.html" class="btn btn-dark btn-lg" role="button"><h3>Sign Up</h3></a>
            </div>
            <div class="col-md-2">
                <a href="../frontstore/aboutus.php" class="btn btn-dark btn-lg" role="button"><h3>About Us</h3></a>
            </div>
            <div class="col-md-2">
                <a href="../frontstore/contactus.php" class="btn btn-dark btn-lg" role="button"><h3>Contact</h3></a>
            </div>
        </div>
    </div>

    <div class="container-md">
        <div class="row colorfill-grey" id="productsContainer">

            <form action="../inventory_delete_handler.php" method="post" id="form">
                <input name="product_name" type="hidden" id="input_product_name"/>
            </form>

            <!-- product cards go here-->

            <div id="emptyContainer"></div>
        </div>

    </div>
</main>

</body>
<script src="https://cdn.jsdelivr.net/npm/bootstrap@5.1.3/dist/js/bootstrap.bundle.min.js"
        integrity="sha384-ka7Sk0Gln4gmtz2MlQnikT1wXgYsOg+OMhuP+IlRH9sENBO0LRn5q+8nbTov4+1p"
        crossorigin="anonymous"></script>

<script src="index.js" rel="script"></script>


</html>