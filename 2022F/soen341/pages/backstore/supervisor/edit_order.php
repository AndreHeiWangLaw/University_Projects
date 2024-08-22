<?php
session_start();

if ((strcasecmp($_SESSION['role'], 'supervisor') != 0))
    echo "<script type='text/javascript'>document.location.replace(\"../../frontstore/index.php\");</script>";
elseif (isset($_GET['username'])){
    $username = $_GET['username'];
    if (strcasecmp($_SESSION['username'],$username) != 0)
        echo "<script type='text/javascript'>document.location.replace(\"../../frontstore/index.php\");</script>";
}

$order_id = $_GET['id'];
$total_price = 0;
?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8"/>
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

        .order-row {
        }
    </style>
    <title>Groscerise</title>
</head>

<body>
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
                <span class="link-text logo-text"></span>
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
            <a href="purchases.php" class="nav-link">
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

<main>
    <div class="container-md colorfill-orange">
        <div class="row transparent">
            <div class="col-md-4">
                <a href="purchases.php" class="btn btn-dark btn-lg" role="button"><h3>Exit Editing</h3></a>
            </div>
            <?php
            echo "<div class=\"col-md-4\"><h1>Order #$order_id</h1></div>"; ?>
            <div class="col-md-4"><h1 style="color:red">Unpaid</h1></div>
        </div>
    </div>

    <form>
        <div class="container-md">
            <div class="row colorfill-grey">
                <div class="col-md-3"><h5>Item</h5></div>
                <div class="col-md-3"><h5>Id</h5></div>
                <div class="col-md-2"><h5>Quantity</h5></div>
                <div class="col-md-2"><h5>Total</h5></div>
                <div class="col-md-2"><h5>Action</h5></div>
            </div>

            <!-- Product rows are displayed here -->
            <?php
            if (strcmp($order_id, 'new') == 0) {
                $isnew = true;
                $order_id = get_unset_id("../database/orders.json");
            } else {
                $isnew = false;
                $products = get_products_by_id("../database/orders.json", $order_id);

                foreach ($products as $product) {
                    $product_id = $product['product_id'];
                    $quantity = intval($product['quantity']);
                    $product_name = get_prod_property("../database/inventory.json", $product_id, "product_name");
                    $img_src = get_prod_property("../database/inventory.json", $product_id, "img_src");
                    $unit_price = get_prod_property("../database/inventory.json", $product_id, "product_price");
                    $product_total_price = $unit_price * $quantity;
                    $total_price += $product_total_price;

                    echo "<div class=\"row colorfill-grey align-items-center order-row\" id='$product_id'>
                                <div class=\"col-md-3\">
                                    <img src=\"../$img_src\" class=\"img-thumbnail img-responsive\">
                                </div>
                                <div class=\"col-md-1\"><h5>$product_name</h5></div>
                                <div class=\"col-md-2\"><input type='text' size='1' name='product_id' value='$product_id'></div>
                                <div class=\"col-md-2\">
                                    <div class=\"increment-wrapper\">
                                        <span onclick=\"decreaseAmount('$product_id" . "_quantity', $unit_price)\" style=\"cursor: pointer\" class=\"minus\">-</span>
                                        <label class=\"number\" id=\"$product_id" . "_quantity\">$quantity</label>
                                        <span onclick=\"increaseAmount('$product_id" . "_quantity', $unit_price)\" style=\"cursor: pointer\" class=\"plus\">+</span>
                                    </div>
                                </div>
                                <div class=\"col-md-2\"><h5 id='$product_id" . "_quantity_total'>\$$product_total_price</h5></div>
                                <div class=\"col-md-2\">
                                    <button type=\"button\" class=\"btn btn-outline-dark\" onclick='deleteRow($product_id)'>
                                        <i class=\"fa fa-trash-o\" style=\"font-size:24px\"></i>
                                    </button>
                                </div>
                        </div>";
                }
            }
            ?>

            <div class="row colorfill-grey align-items-center" id='$product_id'>
                <div class="col-md-4"><h5>New Product</h5></div>
                <div class="col-md-4"><input type='text' size='10' id="newProductId" placeholder="Product Id"></div>
                <div class="col-md-4">
                    <?php
                    echo "<button type=\"button\" class=\"btn btn-outline-dark\" onclick=\"addRow($order_id)\">
                        <h5>Add</h5>
                    </button>"
                    ?>
                </div>
            </div>

            <!-- The total is displayed here -->
            <div class="row colorfill-grey justify-content-end align-items-center">
                <div class="col-md-2"><h5>Total</h5></div>
                <?php
                echo "<div class=\"col-md-2\"><h5>\$<span id='totalPrice'>$total_price</span></h5></div>";
                ?>
                <div class="col-md-2"></div>
            </div>
        </div>

        </div>

        <div class="container-md colorfill-orange">
            <?php
            if (!$isnew) {
                $userid = get_property_by_id("../database/orders.json", $order_id, 'userId');
                echo "<label>User Id: </label><input type=\"text\" id=\"userId\" placeholder=\"User Id\" value='$userid'>";
            }
            else
                echo "<label>User Id: </label><input type=\"text\" id=\"userId\" placeholder=\"User Id\">";
            ?>

            <div class="col-md-12">
                <?php
                echo "<input class=\"btn btn-outline-dark btn-lg textcolor\" value=\"Save Changes\" onclick=\"submitOrder($order_id);\">";
                ?>
            </div>
        </div>

    </form>
</main>

<script src="edit_order.js" type="text/javascript"></script>
</body>
</html>
