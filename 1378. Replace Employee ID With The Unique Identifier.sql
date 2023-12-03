SELECT EmployeeUNI.unique_id, name
FROM Employees
LEFT JOIN EmployeeUNI
ON EmployeeUNI.id = Employees.id

