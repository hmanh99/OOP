#include<iostream>
#include<iomanip>
#include<cmath>
#include<sstream>
#include<string>
#include<algorithm>
#include<vector>
#include<set>
#include<unordered_set>
#include<map>
#include<unordered_map>
#include<stack>
#include<queue>
#include<memory>

using namespace std;

string normalizeUserID(string type, string id) {
	string tmp = (type == "Customer"  ? "C" : "D");
	if (id.length() >= 5) {
		return tmp + id;
	}
	return tmp + string(5 - id.length(), '0') + id;
}

string normalizeName(string& rawName) {
	string newName = "";
	stringstream ss(rawName);
	string token;

	while (ss >> token) {
		newName += toupper(token[0]);
		for (int i = 1; i < token.length(); i++) {
			newName += tolower(token[i]);
		}
		newName += " ";
	}
	if(!newName.empty()){
		newName.pop_back();
	}
	return newName;
}

bool isValidID(const string& id) {
	if (id.empty()) return false;

	bool hasLetter = false;
	bool hasDigit = false;

	for (char c : id) {
		if (isalpha(c)) hasLetter = true;
		if (isdigit(c)) hasDigit = true;
	}

	return hasLetter && hasDigit;
}

bool isValidName(const string& name) {
	if (name.empty()) return false;

	for (char c : name) {
		if (!isalpha(c) && !isspace(c)) {
			return false;
		}
	}

	return true;
}

bool isValidPhone(const string& phone) {
	if (phone.length() != 10) return false;
	if (phone[0] != '0') return false;
	for (char c : phone) {
		if (!isdigit(c)) return false;
	}
	return true;
}

class User {
protected: 
	string userID;
	string name;
	string phone;
	string password;
public:
	User() {}

	User(string id, string n, string p, string pw) {
		userID = id;
		name = n;
		phone = p;
		password = pw;
	}

	virtual ~User() {}
	
	//read_only
	string getUserID() const{
		return userID;
	}
	string getName() const {
		return name;
	}
	string getPhone() const {
		return phone;
	}
	bool authenticate(string pw) const {
		return password == pw;
	}


	virtual void displayInfo() const {
		cout << "ID: " << userID << endl;
		cout << "Name: " << name << endl;
		cout << "Phone: " << phone << endl;

	}

	virtual string getType() const = 0; // 1 -> costumer / 2 ->driver
};

class Vehicle {
private:
	string licensePlateNumber;
	string brand;
	string model;
	int capacity;
	long long pricePerKm;
public:
	Vehicle(string lpn, string b, string m, int c, long long price) {
		licensePlateNumber = lpn;
		brand = b;
		model = m;
		capacity = c;
		pricePerKm = price;
	}

	string getLicensePlateNumber() const {
		return licensePlateNumber;
	}
	string getBrand() const {
		return brand;
	}
	string getModel() const {
		return model;
	}
	int getCapacity() const {
		return capacity;
	}
	double getPrice() const {
		return pricePerKm;
	}

	void displayInformation() const {
		cout << "Bien so: " << licensePlateNumber << endl;
		cout << "Xe: " << brand << " " << model << endl;
		cout << "Suc chua: " << capacity << endl;
		cout << "Gia (VND/km): " << pricePerKm << endl;
	}

};

class Driver : public User {
private:
	shared_ptr<Vehicle> vehicle;
	double rating;
	int totalTrips;
	bool available;
public:
	Driver(string id, string n, string p, string pw) : User(id, n, p, pw) {
		rating = 5.0;
		totalTrips = 0;
		available = false;
	}

	void assignVehicle(shared_ptr<Vehicle> v) {
		vehicle = v;
		available = true;
	}

	shared_ptr<Vehicle> getVehicle() const { 
		return vehicle;
	}

	void setAvailablity(bool a){
		available = a;
	}
	bool getAvailablity() const {
		return available;
	}

	double getRating() const { 
		return rating;
	}
	int getTotalTrips() const { 
		return totalTrips;
	}

	void updateRating(double newRating) {
		rating = ((rating * totalTrips) + newRating) / (totalTrips + 1);
		totalTrips++;
	}

	void displayInfo() const override {
		cout << "\n==================================================\n";
		cout << "                 THONG TIN TAI XE                  \n";
		cout << "==================================================\n";
		User::displayInfo();
		cout << "Danh gia: " << rating << endl;
		cout << "Tong so chuyen di: " << totalTrips << endl;
		cout << "Trang thai: " << (available ? "San sang" : "Ban") << endl;
		
		if (vehicle) {
			cout << "\n==================================================\n";
			cout << "                  THONG TIN XE                   \n";
			vehicle->displayInformation();
		}
		cout << "\n==================================================\n";
	}

	string getType() const override {
		return "Driver";
	}
};

class Customer : public User {
private:
	vector<int> tripHistory;
public:
	Customer(string id, string n, string p, string pw) : User(id, n, p, pw) {
		tripHistory = {};
	}

	void addTripToHistory(int tripID) {
		tripHistory.push_back(tripID);
	}

	vector<int> getTripHistory() const {
		return tripHistory;
	}

	void displayInfo() const override {
		cout << "\n--------------------------------------------------\n";
		cout << "                THONG TIN KHACH HANG                \n";
		cout << "--------------------------------------------------\n";
		User::displayInfo();
		cout << "Tong so chuyen da di: " << tripHistory.size() << endl;
		cout << "--------------------------------------------------\n";
	}

	string getType() const override {
		return "Customer";
	}
};

class Trip {
private:
	static int nextID;
	int tripID;
	shared_ptr<Customer> customer;
	shared_ptr<Driver> driver;
	string pickupLocation;
	string dropLocation;
	double distance;
	long long fare;
	string status; // (Pending/Completed/Cancelled)
	double customerRating;

public:
	Trip(shared_ptr<Customer> c, string pickup, string drop, double dist) {
		tripID = nextID++;
		customer = c;
		pickupLocation = pickup;
		dropLocation = drop;
		distance = dist;
		fare = 0;
		status = "Pending";
		customerRating = 0;
	}

	int getTripID() const {
		return tripID;
	}

	shared_ptr<Customer> getCustomer() const {
		return customer;
	}

	shared_ptr<Driver> getDriver() const {
		return driver;
	}

	string getPickupLocation() const {
		return pickupLocation;
	}

	string getDropLocation() const {
		return dropLocation;
	}

	string getStatus() const {
		return status;
	}

	double getDistance() const {
		return distance;
	}

	long long getFare() const {
		return fare;
	}

	void assignDriver(shared_ptr<Driver> d) {
		driver = d;
		if (driver && driver->getVehicle()) {
			fare = distance * driver->getVehicle()->getPrice();
		}
		driver->setAvailablity(false);
	}

	void tripCompleted(double rating) {
		status = "Completed";
		customerRating = rating;
		if (driver) {
			driver->updateRating(customerRating);
			driver->setAvailablity(true);
		}
	}

	void tripCancelled() {
		status = "Cancelled";
		if (driver) {
			driver->setAvailablity(true);
		}
	}

	void displayInfo() const {
		cout << "\n--------------------------------------------------\n";	
		cout <<   "Chuyen di #" + string(5 - to_string(tripID).length(), '0') + to_string(tripID) << endl;
		cout <<    "--------------------------------------------------\n";
		cout << "| Khach: " << customer->getName() << endl;
		
		if (driver) {
			cout << "| Tai xe: " << driver->getName() << endl;
			if (driver->getVehicle()) {
				cout << "Xe: " << driver->getVehicle()->getBrand() << " " << driver->getVehicle()->getModel() << endl;
				cout << "Bien so: " << driver->getVehicle()->getLicensePlateNumber() << endl;
			}
		}

		cout << "Diem don: " << pickupLocation << endl;
		cout << "Diem den: " << dropLocation << endl;
		cout << "Khoang cach: " << fixed << setprecision(1) << distance << "km" << endl;
		cout << "Gia cuoc: " << fare << "VND" << endl;
		cout << "Trang thai: " << status << endl;
		
		if (status == "Completed") {
			cout << "Danh gia: " << fixed << setprecision(1) << customerRating << "*" << endl;
		}

		cout << "--------------------------------------------------\n";
	}
};

int Trip::nextID = 1;

class BookingSystem {
private: 
	map<string, shared_ptr<Customer>> customers;
	map<string, shared_ptr<Driver>> drivers;
	vector<shared_ptr<Trip>> trips;
	shared_ptr<User> currentUser;

	double calculateDistance(string from, string to) {
		int hash1 = 0, hash2 = 0;
		for (char c : from) hash1 += c;
		for (char c : to) hash2 += c;
		return abs(hash1 - hash2) % 200 + 1.0; // 1-201 km
	}

public:
	BookingSystem() : currentUser(nullptr) {}

	bool registerUser(string type, string id, string name, string phone, string password) {
		id = normalizeUserID(type, id);
		name = normalizeName(name);

		if (!isValidID(id)) {
			cout << "ID khong hop le! ID chi duoc chua chu cai va so.\n";
			return false;
		}

		if (!isValidName(name)) {
			cout << "Ten chi duoc chua khoang trang va chu cai\n";
			return false;
		}

		if (!isValidPhone(phone)) {
			cout << "So dien thoai khong hop le!\n";
			return false;
		}
		if (type == "Customer") {
			if (customers.find(id) != customers.end()) {
				cout << "ID da ton tai!\n";
				return false;
			}

			customers[id] = make_shared<Customer>(id, name, phone, password);
			cout << "Da dang ki thanh cong! \n";
			cout << "ID: " << id << endl;
			cout << "Ten: " << name << endl;
			cout << "SDT: " << phone << endl;
			
			return true;
		}
		else if (type == "Driver") {
			if (drivers.find(id) != drivers.end()) {
				cout << "ID da ton tai!\n";
				return false;
			}

			drivers[id] = make_shared<Driver>(id, name, phone, password);
			cout << "Da dang ki thanh cong! \n";
			cout << "ID: " << id << endl;
			cout << "Ten: " << name << endl;
			cout << "SDT: " << phone << endl;
			
			return true;
		}
		return false;
	}

	bool login(string type, string id, string password) {
		id = normalizeUserID(type, id);

		if (password.empty()) {
			cout << "Mat khau khong duoc de trong!\n";
			return false;
		}

		if (type == "Customer" && customers.find(id) != customers.end()) {
			if (customers[id]->authenticate(password)) {
				currentUser = customers[id];
				cout << "Dang nhap thanh cong! Xin chao " << customers[id]->getName() << "\n";
				return true;
			}
		}

		if (type == "Driver" && drivers.find(id) != drivers.end()) {
			if (drivers[id]->authenticate(password)) {
				currentUser = drivers[id];
				cout << "Dang nhap thanh cong! Xin chao " << drivers[id]->getName() << "\n";
				return true;
			}
		}

		cout << "ID hoac Mat khau khong dung! \n";
		return false;
	}

	void logout() {
		if (currentUser) {
			cout << "Da dang xuat!\n";
			currentUser = nullptr;
		}
	}

	bool registerVehicle(string driverID, string plate, string brand, string model, int capacity, long long price) {
		driverID = normalizeUserID("Driver", driverID);

		if (drivers.find(driverID) == drivers.end()) {
			cout << "Khong ton tai tai xe voi ID: " << driverID << endl;
			return false;
		}

		//auto vehicle = make_shared<Vehicle>(plate, brand, model, capacity, price);
		shared_ptr<Vehicle> vehicle = make_shared<Vehicle>(plate, brand, model, capacity, price);
		drivers[driverID]->assignVehicle(vehicle);
		cout << "Dang ky xe thanh cong!\n";
		return true;
	}

	bool bookTrip(string pickup, string drop) {
		if (!currentUser || currentUser->getType() != "Customer") {
			cout << "Hay dang nhap voi tai khoan khach hang! \n";
			return false;
		}

		//auto customer = dynamic_pointer_cast<Customer>(currentUser);
		shared_ptr<Customer> customer = dynamic_pointer_cast<Customer>(currentUser);
		double distance = calculateDistance(pickup, drop);

		auto trip = make_shared<Trip>(customer, pickup, drop, distance);

		shared_ptr<Driver> availableDriver = nullptr;
		for (auto& driver : drivers) {
			if (driver.second->getAvailablity() && driver.second->getVehicle()) {
				availableDriver = driver.second;
			}
		}

		if (!availableDriver) {
			cout << "Hien khong co tai xe kha dung!\n";
			return false;
		}

		trip->assignDriver(availableDriver);
		trips.push_back(trip);
		customer->addTripToHistory(trip->getTripID());
		
		cout << "\nDat xe thanh cong!\n";
		trip->displayInfo();

		return true;
	}

	bool tripCompleted(int tripID, double rating) {
		if (rating < 1.0 || rating > 5.0) {
			cout << "Danh gia phai tu 1-5!\n";
			return false;
		}
		
		for (auto& trip : trips) {
			if (trip->getTripID() == tripID && trip->getStatus() == "Pending") {
				trip->tripCompleted(rating);
				cout << "Hoan thanh chuyen di!\n";
				return true;
			}
		}
		cout << "Khong tim thay chuyen di!\n";
		return false;
	}

	void displayAllDrivers() const {
		cout << "\n--------------------------------------------------\n";
		cout <<   "                 DANH SACH TAI XE                 " << endl;
		cout << "--------------------------------------------------\n";

		if (drivers.empty()) {
			cout << "Chua co tai xe nao trong he thong dat xe.\n";
		}

		for (const auto& driver : drivers) {
			driver.second->displayInfo();
		}
	}

	void displayTripHistory() const {
		if (!currentUser) {
			cout << "Vui long dang nhap!\n";
			return;
		}
		if (currentUser->getType() == "Customer") {
			auto customer = dynamic_pointer_cast<Customer>(currentUser);
			vector<int> history = customer->getTripHistory();

			if (history.empty()) {
				cout << "Chua co chuyen di nao.\n";
				return;
			}

			for (int tripID : history) {
				for (const auto& trip : trips) {
					if (trip->getTripID() == tripID) {
						trip->displayInfo();
					}
				}
			}
		}
		else {
			auto driver = dynamic_pointer_cast<Driver>(currentUser);
			bool found = false;

			for (const auto& trip : trips) {
				if (trip->getDriver() && trip->getDriver()->getUserID() == driver->getUserID()) {
					trip->displayInfo();
					found = true;
				}
			}

			if (!found) {
				cout << "Chua co chuyen di nao.\n";
			}
		}
	}

	void displayCurrentUserInfo() const {
		if (currentUser) {
			currentUser->displayInfo();
		}
		else {
			cout << "Chua dang nhap!\n";
		}
	}

	shared_ptr<User> getCurrentUser() const { 
		return currentUser;
	}
};

void displayMenu() {
	cout << "\n";
	cout << " ______________________________________________ \n";
	cout << "|               HE THONG DAT XE                |\n";
	cout << "|----------------------------------------------|\n";
	cout << "| 1. Dang ky                                   |\n";
	cout << "| 2. Dang nhap                                 |\n";
	cout << "| 3. Thoat                                     |\n";
	cout << "|______________________________________________|\n";
	cout << "Chon: ";
}

void displayCustomerMenu() {
	cout << "\n";
	cout << " ______________________________________________ \n";
	cout << "|               MENU KHACH HANG                |\n";
	cout << "|----------------------------------------------|\n";
	cout << "| 1. Dat xe                                    |\n";
	cout << "| 2. Xem lich su chuyen di                     |\n";
	cout << "| 3. Xem thong tin ca nhan                     |\n";
	cout << "| 4. Hoan thanh chuyen di                      |\n";
	cout << "| 5. Dang xuat                                 |\n";
	cout << "|______________________________________________|\n";
	cout << "Chon: ";
}

void displayDriverMenu() {
	cout << "\n";
	cout << " ______________________________________________\n";
	cout << "|                  MENU TAI XE                 |\n";
	cout << "|----------------------------------------------|\n";
	cout << "| 1. Xem lich su chuyen di                     |\n";
	cout << "| 2. Xem thong tin ca nhan                     |\n";
	cout << "| 3. Xem danh sach tai xe                      |\n";
	cout << "| 4. Dang xuat                                 |\n";
	cout << "|______________________________________________|\n";
	cout << "Chon: ";
}

int main() {

	BookingSystem system;

	system.registerUser("Driver", "1", "Nguyen Van A", "0901234567", "123456");
	system.registerVehicle("1", "29A-12345", "Toyota", "Vios", 4, 10000);

	system.registerUser("Driver", "2", "Tran Van B", "0902345678", "123456");
	system.registerVehicle("2", "30B-67890", "Honda", "City", 4, 12000);

	system.registerUser("Driver", "3", "Le Thi C", "0903456789", "123456");
	system.registerVehicle("3", "31C-11111", "Mazda", "3", 4, 15000);

	system.registerUser("Customer", "1", "Pham Van D", "0904567890", "123456");

	int choice;
	while (true) {
		if (!system.getCurrentUser()) {
			displayMenu();
			cin >> choice;
			cin.ignore();
			
			if (choice == 1) {
				string type, id, name, phone, password;
				cout << "\n--- DANG KY ---\n";
				cout << "Loai tai khoan (Customer/Driver): ";
				getline(cin, type);
				cout << "ID: ";
				getline(cin, id);
				cout << "Ho ten: ";
				getline(cin, name);
				cout << "So dien thoai: ";
				getline(cin, phone);
				cout << "Mat khau: ";
				getline(cin, password);

				if (system.registerUser(type, id, name, phone, password)) {
					if (type == "Driver") {
						string plate, brand, model;
						int capacity;
						double price;

						cout << "\n--- DANG KY XE ---\n";
						cout << "Bien so: ";
						getline(cin, plate);
						cout << "Hang xe: ";
						getline(cin, brand);
						cout << "Model: ";
						getline(cin, model);
						cout << "So cho: ";
						cin >> capacity;
						cout << "Gia/km (VND): ";
						cin >> price;
						cin.ignore();

						system.registerVehicle(id, plate, brand, model, capacity, price);
					}
				}
			}
			else if (choice == 2) {
				string type, id, password;
				cout << "\n--- DANG NHAP ---\n";
				cout << "Loai tai khoan (Customer/Driver): ";
				getline(cin, type);
				cout << "ID: ";
				getline(cin, id);
				cout << "Mat khau: ";
				getline(cin, password);

				system.login(type, id, password);
			}
			else if (choice == 3) {
				cout << "\nCam on ban da su dung dich vu!\n";
				break;
			}
			else{
				cout << "\nCam on ban da su dung dich vu!\n";
				break;
			}
		}
		else {
			
			if (system.getCurrentUser()->getType() == "Customer") {
				displayCustomerMenu();
				cin >> choice;
				cin.ignore();

				if (choice == 1) {
					
					string pickup, dropoff;
					cout << "\n--- DAT XE ---\n";
					cout << "Diem don: ";
					getline(cin, pickup);
					cout << "Diem den: ";
					getline(cin, dropoff);

					system.bookTrip(pickup, dropoff);
				}
				else if (choice == 2) {
					system.displayTripHistory();
				}
				else if (choice == 3) {
					system.displayCurrentUserInfo();
				}
				else if (choice == 4) {
					int tripID;
					double rating;
					cout << "\n--- HOAN THANH CHUYEN DI ---\n";
					cout << "Ma chuyen di: ";
					cin >> tripID;
					cout << "Danh gia (1-5): ";
					cin >> rating;
					cin.ignore();

					system.tripCompleted(tripID, rating);
				}
				else if (choice == 5) {
					system.logout();
				}
			}
			else {
				displayDriverMenu();
				cin >> choice;
				cin.ignore();

				if (choice == 1) {
					system.displayTripHistory();
				}
				else if (choice == 2) {
					system.displayCurrentUserInfo();
				}
				else if (choice == 3) {
					system.displayAllDrivers();
				}
				else if (choice == 4) {
					system.logout();
				}
			}
		}
	}
	return 0;
}
