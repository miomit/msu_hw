-- Вывести всех клиентов и их телефоны
SELECT LastName, FirstName, Phone
FROM Client;

-- Вывести все автомобили и их владельцев
SELECT c.LastName, c.FirstName, car.Brand, car.PlateNumber
FROM Car car
JOIN Client c ON car.ClientID = c.ClientID;

-- Найти всех работников со стажем больше 10 лет
SELECT LastName, FirstName, ExperienceYears
FROM Worker
WHERE ExperienceYears > 10;

-- Вывести все заказы, которые ещё в работе
SELECT OrderID, ClientID, CarID, Status
FROM Orders
WHERE Status = 'В работе';

-- Посчитать количество клиентов
SELECT COUNT(*) AS TotalClients
FROM Client;

-- Посчитать количество автомобилей каждого цвета
SELECT Color, COUNT(*) AS CountCars
FROM Car
GROUP BY Color;

-- Найти среднюю стоимость работ по всем услугам
SELECT AVG(WorkCost) AS AvgWorkCost
FROM OrderService;

-- Найти клиентов, у которых больше одного автомобиля
SELECT c.LastName, c.FirstName, COUNT(car.CarID) AS CarsOwned
FROM Client c
JOIN Car car ON c.ClientID = car.ClientID
GROUP BY c.ClientID
HAVING COUNT(car.CarID) > 1;

-- Найти работников, участвовавших в ремонте заказа №5
SELECT w.LastName, w.FirstName, w.Specialty
FROM OrderWorker ow
JOIN Worker w ON ow.WorkerID = w.WorkerID
WHERE ow.OrderID = 5;

-- Найти среднее количество часов работы по каждой услуге
SELECT s.Name, AVG(os.WorkHours) AS AvgHours
FROM Service s
JOIN OrderService os ON s.ServiceID = os.ServiceID
GROUP BY s.ServiceID;

-- Вывести клиентов, которые сдавали авто в ремонт более 2 раз
SELECT c.LastName, c.FirstName, COUNT(o.OrderID) AS OrdersCount
FROM Client c
JOIN Orders o ON c.ClientID = o.ClientID
GROUP BY c.ClientID
HAVING COUNT(o.OrderID) > 2;

-- Найти автомобили, которые ремонтировались хотя бы один раз
SELECT DISTINCT car.Brand, car.PlateNumber
FROM Car car
JOIN Orders o ON car.CarID = o.CarID;

-- Найти общую стоимость всех работ по заказу №7
SELECT SUM(WorkCost + PartsCost) AS TotalCost
FROM OrderService
WHERE OrderID = 7;
