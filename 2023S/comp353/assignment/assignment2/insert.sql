-- Insert data into the Member table
INSERT INTO Member (memberId, firstName, lastName, middleName, dateOfBirth, address, gender, phoneNumber, email, socialInsuranceNumber, startDateMembership, memberType)
VALUES
    (1, 'John', 'Doe', 'A', '1990-05-15', '123 Main St', 'Male', '555-123-4567', 'john.doe@example.com', '123-45-6789', '2022-01-01', 'client'),
    (2, 'Jane', 'Smith', 'B', '1985-08-25', '456 Elm St', 'Female', '555-987-6543', 'jane.smith@example.com', '987-65-4321', '2021-05-10', 'donor'),
    (3, 'Mike', 'Johnson', 'C', '1980-11-10', '789 Oak St', 'Male', '555-111-2222', 'mike.johnson@example.com', '456-78-9012', '2020-02-15', 'employee');

-- Insert data into the Donation table
INSERT INTO Donation (donationId, donorId, date, type, amount, memberId)
VALUES
    (1, 2, '2023-06-05', 'money', 500, 2),
    (2, 2, '2023-06-15', 'product', 0, 2),
    (3, 1, '2023-06-20', 'money', 1000, 1),
    (4, 3, '2023-06-25', 'product', 0, 3);

-- Insert data into the Product table
INSERT INTO Product (productId, description, date, sellingPrice, weight, inStock)
VALUES
    (1, 'Shirt', '2023-06-15', 20, 0.3, true),
    (2, 'Book', '2023-06-25', 10, 0.5, true),
    (3, 'Chair', '2023-06-30', 50, 5, true);

-- Insert data into the Sales table
INSERT INTO Sales (salesId, date, amount, memberId)
VALUES
    (1, '2023-06-15', 30, 1),
    (2, '2023-06-25', 10, 2),
    (3, '2023-06-30', 100, 3);

-- Insert data into the SalesItems table
INSERT INTO SalesItems (salesItemId, productId, salesId)
VALUES
    (1, 1, 1),
    (2, 2, 2),
    (3, 3, 3);

-- Insert data into the Expenses table
INSERT INTO Expenses (expenseId, presidentId, date, amount, type, description, memberId)
VALUES
    (1, 1, '2023-06-05', 500, 'rent', 'Office Rent', 1),
    (2, 1, '2023-06-10', 200, 'bill', 'Utility Bill', 1),
    (3, 1, '2023-06-15', 100, 'charity', 'Donation to Shelter', 1);

-- Insert data into the Employee table
INSERT INTO Employee (employeeId, memberId, jobTitle, salary)
VALUES
    (1, 1, 'president', 5000),
    (2, 3, 'cashier', 3000);
