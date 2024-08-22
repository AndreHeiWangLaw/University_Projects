<?php
session_start();
if ((strcasecmp($_SESSION['role'], 'supervisor') != 0))
    echo "<script type='text/javascript'>document.location.replace(\"../../frontstore/index.php\");</script>";
elseif (isset($_GET['username'])){
    $username = $_GET['username'];
    if (strcasecmp($_SESSION['username'],$username) != 0)
        echo "<script type='text/javascript'>document.location.replace(\"../../frontstore/index.php\");</script>";
}
require("../../../php/util/order_reader.php");
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
    <link rel="stylesheet" href="../../css/store/styles_backstore.css"/>
    <style>
        @import url("https://fonts.googleapis.com/css2?family=Urbanist:wght@200&display=swap");
    </style>
    <title>List of orders</title>
</head>
<body>
<main>
    <div class="container-md colorfill-orange">
        <div class="row transparent">
            <h1 style="color: black;">Purchases</h1>
        </div>
    </div>

    <div class="container-md">
        <div class="row colorfill-grey">
            <div class="col-md-3 d-none d-sm-none d-md-block"><h5>Order #</h5></div>
            <div class="col-md-3 d-none d-sm-none d-md-block"><h5>User Id</h5></div>
            <div class="col-md-3 d-none d-sm-none d-md-block"><h5>Total</h5></div>
            <div class="col-md-1 d-none d-sm-none d-md-block"><h5>Edit</h5></div>
            <div class="col-md-1 d-none d-sm-none d-md-block"><h5>Delete</h5></div>
            <div class="col-md-1 d-none d-sm-none d-md-block"><h5>Status</h5></div>
        </div>

        <?php
        $orders = get_orders_json_array("../../../database/orders.json");

        foreach ($orders as $order) {
            $order_id = $order['id'];
            $user_id = $order['userId'];
            $total = $order['total'];
            $status = $order['status'];

            echo "<div class=\"row colorfill-grey align-items-center\" id=\"orderContainer\">
                    <div class=\"col-md-3\">
                        <h5>$order_id</h5>
                    </div>
                    <div class=\"col-md-3\">
                        <h5>#$user_id</h5>
                    </div>
                    <div class=\"col-md-3\">
                        <h5>\$$total</h5>
                    </div>
                    <div class=\"col-md-1\">
                        <button type=\"button\" class=\"btn\" onclick=\"document.location = 'edit_order.php?id=' + $order_id \">
                            <i class=\"fa fa-edit\" style=\"font-size:24px\"></i>
                        </button>
                    </div>
                    <div class=\"col-md-1\">
                        <button type=\"button\" class=\"btn\" onclick='deleteOrder($order_id )' id='$order_id'>
                            <i class=\"fa fa-trash-o\" style=\"font-size:24px\"></i>
                        </button>
                    </div>
                    <div class=\"col-md-1\">
                        <h5>$status</h5>
                    </div>
                </div>";
        }


        ?>

    </div>

    <div class="container-md">
        <div class="row transparent">
            <div class="col-md-10"></div>
            <div class="col-md-1">
                <button type="button" class="btn btn-outline-dark btn-lg">
                    <a href="edit_order.php?id=new">
                        <h1 class="textcolor">Add</h1>
                    </a>
                </button>
            </div>
        </div>
    </div>

</main>
<nav class="navbar">
    <ul class="navbar-nav">
        <li class="logo">
            <a href="#" class="nav-link">
                <?php
                if (isset($_SESSION['username'])) {
                    $username = $_SESSION['username'];
                    echo "<span class=\"link-text logo-text\">$username</span>";
                }
                ?>
                <svg
                        aria-hidden="true"
                        focusable="false"
                        data-prefix="fad"
                        data-icon="angle-double-right"
                        role="img"
                        xmlns="http://www.w3.org/2000/svg"
                        viewBox="0 0 448 512"
                        class="svg-inline--fa fa-angle-double-right fa-w-14 fa-5x"
                >
                    <g class="fa-group">
                        <path
                                fill="currentColor"
                                d="M224 273L88.37 409a23.78 23.78 0 0 1-33.8 0L32 386.36a23.94 23.94 0 0 1 0-33.89l96.13-96.37L32 159.73a23.94 23.94 0 0 1 0-33.89l22.44-22.79a23.78 23.78 0 0 1 33.8 0L223.88 239a23.94 23.94 0 0 1 .1 34z"
                                class="fa-secondary"
                        ></path>
                        <path
                                fill="currentColor"
                                d="M415.89 273L280.34 409a23.77 23.77 0 0 1-33.79 0L224 386.26a23.94 23.94 0 0 1 0-33.89L320.11 256l-96-96.47a23.94 23.94 0 0 1 0-33.89l22.52-22.59a23.77 23.77 0 0 1 33.79 0L416 239a24 24 0 0 1-.11 34z"
                                class="fa-primary"
                        ></path>
                    </g>
                </svg>
            </a>
        </li>


        <li class="nav-item">
            <?php
            if (isset($_SESSION['username'])) {
                $username = strtolower($_SESSION['username']);
                echo "<a href=\"purchases.php?username=$username\" class=\"nav-link\">";
            }
            ?>
            <svg
                    aria-hidden="true"
                    focusable="false"
                    data-prefix="fad"
                    data-icon="space-station-moon-alt"
                    role="img"
                    xmlns="http://www.w3.org/2000/svg"
                    viewBox="0 0 576 512"
                    class="fa-solid:dolly"
            >
                <path
                        fill="currentColor"
                        d="M294.2 277.7c18 5 34.7 13.4 49.5 24.7l161.5-53.8c8.4-2.8 12.9-11.9 10.1-20.2L454.9 47.2c-2.8-8.4-11.9-12.9-20.2-10.1l-61.1 20.4l33.1 99.4L346 177l-33.1-99.4l-61.6 20.5c-8.4 2.8-12.9 11.9-10.1 20.2l53 159.4zm281 48.7L565 296c-2.8-8.4-11.9-12.9-20.2-10.1l-213.5 71.2c-17.2-22-43.6-36.4-73.5-37L158.4 21.9C154 8.8 141.8 0 128 0H16C7.2 0 0 7.2 0 16v32c0 8.8 7.2 16 16 16h88.9l92.2 276.7c-26.1 20.4-41.7 53.6-36 90.5c6.1 39.4 37.9 72.3 77.3 79.2c60.2 10.7 112.3-34.8 113.4-92.6l213.3-71.2c8.3-2.8 12.9-11.8 10.1-20.2zM256 464c-26.5 0-48-21.5-48-48s21.5-48 48-48s48 21.5 48 48s-21.5 48-48 48z"
                        class="fa-primary"
                />
            </svg>
            <span class="link-text">Purchases</span>
            </a>
        </li>

        <li class="nav-item" id="themeButton">
            <a href="../../frontstore/index.php" class="nav-link">
                <svg
                        aria-hidden="true"
                        focusable="false"
                        data-prefix="fad"
                        data-icon="space-station-moon-alt"
                        role="img"
                        xmlns="http://www.w3.org/2000/svg"
                        viewBox="0 0 512 512"
                        class="fa-solid:power-off"
                >
                    <path
                            fill="currentColor"
                            d="M400 54.1c63 45 104 118.6 104 201.9c0 136.8-110.8 247.7-247.5 248C120 504.3 8.2 393 8 256.4C7.9 173.1 48.9 99.3 111.8 54.2c11.7-8.3 28-4.8 35 7.7L162.6 90c5.9 10.5 3.1 23.8-6.6 31c-41.5 30.8-68 79.6-68 134.9c-.1 92.3 74.5 168.1 168 168.1c91.6 0 168.6-74.2 168-169.1c-.3-51.8-24.7-101.8-68.1-134c-9.7-7.2-12.4-20.5-6.5-30.9l15.8-28.1c7-12.4 23.2-16.1 34.8-7.8zM296 264V24c0-13.3-10.7-24-24-24h-32c-13.3 0-24 10.7-24 24v240c0 13.3 10.7 24 24 24h32c13.3 0 24-10.7 24-24z"
                            class="fa-primary"
                    />
                </svg>
                <span class="link-text">Logout</span>
            </a>
        </li>
    </ul>
</nav>

</body>
<script src="list_of_orders.js" type="text/javascript"></script>
<script src="https://cdn.jsdelivr.net/npm/bootstrap@5.1.3/dist/js/bootstrap.bundle.min.js"
        integrity="sha384-ka7Sk0Gln4gmtz2MlQnikT1wXgYsOg+OMhuP+IlRH9sENBO0LRn5q+8nbTov4+1p"
        crossorigin="anonymous"></script>

</html>