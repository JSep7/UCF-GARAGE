/* COP3502 Assignment 1
 This program is written by: Jaime Sepulveda*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXLEN 22

typedef struct RegisteredVehicle{ //for one registered vehicle
    char* license_plate; //to be used for string license plate# of the vehicle
    char* owner_name; //to be used for storing owner name
} RegisteredVehicle;

typedef struct Garage{ //for one garage
    char* garage_name; //to be used for garage name
    int total_capacity; //total capacity of the garage
    int current_count; // number of vehicles in the garage in a particular time
RegisteredVehicle** parked_vehicles; //list of parked vehicles in a particular time
} Garage;

typedef struct Campus{
    Garage** garages; //list of garages in campus
    int total_garages; //number of garages in the campus
    RegisteredVehicle** registered_vehicles; //list of registered vehicles
    int total_registered_vehicles; //total number of registered vehicles
} Campus;

/* 
    Creates a new garage with the specified name and 
    capacity and initializes other variables and 
    makes parked vehicles array based on the capacity
*/
Garage* createGarage(const char *name, int capacity) {
    //allocates memory for garage
    Garage* garage = malloc(sizeof(Garage)); 
    //safety measure to make sure it works as inteded
    if (garage == NULL) 
    {
        printf("Memory allocation for garage failed\n");
        exit(1);
    }

    garage->garage_name = malloc(strlen(name) + 1);
    if (garage->garage_name == NULL) 
    {
        printf("Memory allocation for garage name failed\n");
        exit(1);
    }
    strcpy(garage->garage_name, name);

    garage->total_capacity = capacity;
    garage->current_count = 0;

    garage->parked_vehicles = malloc(sizeof(RegisteredVehicle*) * capacity);
    if (garage->parked_vehicles == NULL) 
    {
        printf("Memory allocation for parked vehicles failed\n");
        exit(1);
    }

    //loops through capacity then sets vehicles index to null
    for (int i = 0; i < capacity; i++) 
    {
        garage->parked_vehicles[i] = NULL;
    }

    return garage;
}

/*
    Create a new vehicle with 
    the given license plate and owner.
*/
RegisteredVehicle* createRegisteredVehicle(const char *license, const char *owner) {
    RegisteredVehicle* vehicle = malloc(sizeof(RegisteredVehicle));
    if (vehicle == NULL) 
    {
        printf("Memory allocation for vehicle failed\n");
        exit(1);
    }
    
    //allocates vehicle license plate memory. +1 is for the null terminator
    vehicle->license_plate = malloc(strlen(license) + 1);
    //if statment if the memory allocation fails
    if (vehicle->license_plate == NULL) 
    {
        printf("Memory allocation for license plate failed\n");
        free(vehicle);
        exit(1);
    }
    strcpy(vehicle->license_plate, license);

    vehicle->owner_name = malloc(strlen(owner) + 1);
    if (vehicle->owner_name == NULL) 
    {
        printf("Memory allocation for owner name failed\n");
        free(vehicle->license_plate); 
        free(vehicle);
        exit(1);
    }
    //copies owner to vehicle owner_name
    strcpy(vehicle->owner_name, owner);

    return vehicle;
}

/*
    Parks a vehicle in a specified garage. 
    If the garage is full, it prints “FULL”. 
    Otherwise, if the parking is successful, it prints “PARKED”.
*/
void parkVehicle(Garage* garage, RegisteredVehicle* vehicle) {
    //if statements to check if current count is greater then or equal to total capacity 
    if (garage->current_count >= garage->total_capacity) 
    {
        printf("FULL\n");
        return;
    }
    
    // Add the vehicle to the next available spot
    garage->parked_vehicles[garage->current_count++] = vehicle;
    printf("PARKED\n");

    return;
}

/*
    Removes a vehicle from a specified garage. 
    The function returns 1 if the removal is successful. 
    Otherwise, it returns 0.
*/
int removeVehicleFromGarage(Garage* garage, const char* license) {
    //for loop thats loops through the garage total capacity
    for (int i = 0; i < garage->total_capacity; i++) 
    {
        //checks if vehicle is there and it matches the license them sets vehicle index to null and lowers count
        if (garage->parked_vehicles[i] != NULL && strcmp(garage->parked_vehicles[i]->license_plate, license) == 0) 
        {
            garage->parked_vehicles[i] = NULL;
            garage->current_count--;
            return 1;
        }
    }
    return 0;
}

/*
    Searches for a vehicle by its license plate across all garages on campus. 
    If the vehicle is found, it return the vehicle. 
    Otherwise, it should return NULL.
*/
RegisteredVehicle* searchVehicleByLicense(const Campus* campus, const char* license) {
    //loops through the campus total registered vehicles and checks if the vehicles mathces the input license then returns vehicle
    for (int i = 0; i < campus->total_registered_vehicles; i++) 
    {
        if (strcmp(campus->registered_vehicles[i]->license_plate, license) == 0) 
        {
            return campus->registered_vehicles[i];
        }
    }
    return NULL;
}

/*
    Returns the total number of vehicles 
    parked across all garages on campus.
*/
int countTotalVehicles(const Campus* campus) {
    int total_count = 0;
    //loops through all vehicles on campus and stores and increments the count
    for (int i = 0; i < campus->total_garages; i++) 
    {
        total_count += campus->garages[i]->current_count;
    }
    return total_count;
}

/*
    Resizes a garage to a new capacity. If the new capacity is smaller 
    than the current number of vehicles in that garage, then you should print a
    massage "FAIL. TOO SMALL TO ACCOMMODATE EXISTING VEHICLES.\n". 
    The function returns 1 or 0 depending on whether the task was successful or not.
*/
int resizeGarage(Garage* garage, int new_capacity) {
    //checks to make sure the new_capacity is not more than current count in garage
    if (new_capacity < garage->current_count)
    {
        printf("FAIL. TOO SMALL TO ACCOMMODATE EXISTING VEHICLES.\n");
        return 0;
    }

    
    RegisteredVehicle** new_parked_vehicles = realloc(garage->parked_vehicles, new_capacity * sizeof(RegisteredVehicle*));
    if (new_parked_vehicles == NULL) 
    {
        printf("Memory allocation failed.\n");
        return 0;
    }
    
    //sets the parked vehicles to the new realloc 
    garage->parked_vehicles = new_parked_vehicles;

    for (int i = garage->total_capacity; i < new_capacity; i++) 
    {
        garage->parked_vehicles[i] = NULL;
    }

    garage->total_capacity = new_capacity;
    printf("SUCCESS\n");
    return 1;
}

/*
    Relocates a vehicle from one garage to another. If the vehicle not found, it prints the message like
    point 7 above. If the target garage is not found, it should print “[G] NOT FOUND”. If the target garage
    is full, it should print “[G] IS FULL.” and should not remove the vehicle from the current garage. The
    function returns 1 or 0 depending on whether the task was successful or not.
*/
int relocateVehicle(Campus* campus, const char* license, const char* target_garage_name) {
    Garage* current = NULL;
    RegisteredVehicle* vehicle = NULL;

    // Find the current garage and vehicle
    for (int i = 0; i < campus->total_garages; i++) 
    {
        Garage* garage = campus->garages[i];

        //inside loop for the vehicles
        for (int j = 0; j < garage->current_count; j++) 
        {
            if (strcmp(garage->parked_vehicles[j]->license_plate, license) == 0) 
            {
                current = garage;
                vehicle = garage->parked_vehicles[j];
                break;
            }
        }

        if (vehicle != NULL) 
        {
            break;
        }
    }

    // Find the target garage
    Garage* target_garage = NULL;
    for (int i = 0; i < campus->total_garages; i++) 
    {
        if (strcmp(campus->garages[i]->garage_name, target_garage_name) == 0) 
        {
            target_garage = campus->garages[i];
            break;
        }
    }

    if (target_garage == NULL) 
    {
        printf("%s NOT FOUND.\n", target_garage_name);
        return 0;
    }
    
     if (vehicle == NULL) 
     {
        printf("%s NOT IN CAMPUS.\n", license);
        return 0;
    }

    // Check if the target garage is full
    if (target_garage->current_count >= target_garage->total_capacity) 
    {
        printf("%s IS FULL.\n", target_garage_name);
        return 0;
    }

    // Remove the vehicle from the current garage
    for (int i = 0; i < current->current_count; i++) 
    {
        if (strcmp(current->parked_vehicles[i]->license_plate, license) == 0) 
        {
            // Shift vehicles to remove the current vehicle
            for (int j = i; j < current->current_count - 1; j++) 
            {
                current->parked_vehicles[j] = current->parked_vehicles[j + 1];
            }
            current->current_count--;
            break;
        }
    }

    // Add the vehicle to the target garage
    target_garage->parked_vehicles[target_garage->current_count] = vehicle;
    target_garage->current_count++;

    printf("PARKED\n");
    printf("RELOCATION SUCCESSFUL.\n");
    return 1;
}

/*
    Displays all vehicles owned by a specified person if they are in campus. 
    It means, same owner can have multiple vehicles 
    and all of them can be on campus as well.
*/
void displayVehiclesByOwner(const Campus* campus, const char* owner_name) {
    int found = 0;  // To track if any vehicles are found for the owner
    
    // Loop through all registered vehicles
    for (int h = 0; h < campus->total_registered_vehicles; h++) 
    {
        // Check if the registered vehicle's owner matches the searched owner name
        if (strcmp(campus->registered_vehicles[h]->owner_name, owner_name) == 0) 
        {
            found++;  // Increment the found counter
            const char* owner_license = campus->registered_vehicles[h]->license_plate;
            int parked = 0;  // To track if the current vehicle is found on campus

            // Loop through all garages
            for (int i = 0; i < campus->total_garages; i++) 
            {
                Garage* garage = campus->garages[i];
                
                // Loop through all parked vehicles in the current garage
                for (int j = 0; j < garage->total_capacity; j++) 
                {
                    if (garage->parked_vehicles[j] != NULL && strcmp(garage->parked_vehicles[j]->license_plate, owner_license) == 0) 
                    {
                        // If the vehicle is found in a garage, print its location
                        printf("%s %s\n", owner_license, garage->garage_name);
                        parked = 1;  // Mark the vehicle as parked
                        break;
                    }
                }
                if(parked)
                {
                    break;
                }
            }

            // If the vehicle is not found in any garage, print not found on campus
            if (!parked) 
            {
                printf("%s NOT ON CAMPUS\n", owner_license);
            }
        }
    }
    
    // If no vehicles are found for the owner
    if (found == 0) 
    {
        printf("NO REGISTERED CAR BY THIS OWNER\n");
    }
}

/*
    Removes a specified garage from the campus and frees the memory. 
    Note that while removing a garage, you should not remove
    the remove the vehicles from the system. The function returns 1 or 0 
    depending on whether the task was successful or not.
*/
int removeGarage(Campus* campus, const char* garage_name) {
    int remove = -1;
    for (int i = 0; i < campus->total_garages; i++) 
    {
        if (strcmp(campus->garages[i]->garage_name, garage_name) == 0) 
        {
            free(campus->garages[i]->garage_name);
            free(campus->garages[i]->parked_vehicles);
            free(campus->garages[i]);
            remove = i;
            break;
        }
    }
    if (remove != -1) 
    {
        for (int j = remove; j < campus->total_garages - 1; j++) 
        {
            campus->garages[j] = campus->garages[j + 1];
        }
          
        campus->total_garages--;
        campus->garages = realloc(campus->garages, sizeof(Garage*) * campus->total_garages); //we need set an array = realloc to allocate memory
        printf("%s REMOVED\n", garage_name);
        return 1;
    }
    printf("%s NOT FOUND\n", garage_name);
    return 0;
}

/*
    Generates a report on the utilization of all garages. For each garage, 
    it should print: “Garage: [G], Capacity: [GC], Occupied: [GO], Utilization: [GU]%”, 
    where [G] is the garage name, [GC] is the capacity of the garage, [GO] is
    the count of vehicles in the garage, and [GU] is the percentage of fullness of the garage. After printing
    the above summary, you should print the name of garage which is utilized least at that time.
*/
void generateGarageUtilizationReport(const Campus* campus) {
    //new variables to store least used garage
    int least_utilized_index = -1;
    float least_utilization = 100.00;

    //for loop that loops through garage prints the capcity and the capacity %
    for (int i = 0; i < campus->total_garages; i++) 
    {
        Garage* garage = campus->garages[i];
        float utilization = ((float)garage->current_count / garage->total_capacity) * 100;
        printf("Garage: %s, Capacity: %d, Occupied: %d, Utilization: %.2f%%\n",
        garage->garage_name, garage->total_capacity, garage->current_count, utilization);

        if (utilization < least_utilization) 
        {
            least_utilization = utilization;
            least_utilized_index = i;
        }
    }

    //checks to see if the least index is valid and prints it
    if (least_utilized_index != -1) 
    {
        printf("Least Utilized: %s\n", campus->garages[least_utilized_index]->garage_name);
    }
}

// Function to free all allocated memory
void free_memory(Campus* campus) {
    // Free all garages
    for (int i = 0; i < campus->total_garages; i++) 
    {
        if (campus->garages[i] != NULL) 
        {
            free(campus->garages[i]->garage_name);         // Free garage name
            free(campus->garages[i]->parked_vehicles);     // Free parked vehicles list
            free(campus->garages[i]);                      // Free the garage struct itself
        }
    }
    free(campus->garages); // Free the array of garage pointers

    // Free all registered vehicles
    for (int j = 0; j < campus->total_registered_vehicles; j++) 
    {
        if (campus->registered_vehicles[j] != NULL) 
        {
            free(campus->registered_vehicles[j]->license_plate); // Free license plate
            free(campus->registered_vehicles[j]->owner_name);    // Free owner name
            free(campus->registered_vehicles[j]);                // Free the vehicle struct itself
        }
    }
    free(campus->registered_vehicles); // Free the array of vehicle pointers
}

int main() {
    int num_garages;
    int num_registered_vehicles;
    int command_count;
    int capacity;
    char commands[MAXLEN], license[MAXLEN], garage_name[MAXLEN], owner[MAXLEN];

    // Read input for number of garages, vehicles, and commands
    scanf("%d %d %d", &num_garages, &num_registered_vehicles, &command_count);

    // Initialize the campus structure
    Campus campus;
    campus.registered_vehicles = malloc(sizeof(RegisteredVehicle*) * num_registered_vehicles);
    campus.total_garages = num_garages;
    campus.total_registered_vehicles = num_registered_vehicles;
    campus.garages = malloc(sizeof(Garage*) * num_garages);

    // Read garage details and create each garage
    for (int i = 0; i < num_garages; i++) 
    {
        scanf("%s %d", garage_name, &capacity);
        campus.garages[i] = createGarage(garage_name, capacity);
    }

    // Read vehicle details and register each vehicle
    for (int j = 0; j < num_registered_vehicles; j++) 
    {
        scanf("%s %s", license, owner);
        campus.registered_vehicles[j] = createRegisteredVehicle(license, owner);
    }

    // for loops that goes through all the commands and uses the functions accordingly
    for (int i = 0; i < command_count; i++) 
    {
        scanf("%s", commands);

        if (strcmp(commands, "PARK") == 0) 
        {
            scanf("%s %s", license, garage_name);
            RegisteredVehicle* vehicle = searchVehicleByLicense(&campus, license);
            
            if (vehicle == NULL) 
            {
                printf("Vehicle not registered.\n");
                continue;
            }

            int found_garage = 0;
            for (int k = 0; k < num_garages; k++) 
            {
                if (strcmp(campus.garages[k]->garage_name, garage_name) == 0) 
                {
                    found_garage = 1;
                    parkVehicle(campus.garages[k], vehicle);
                    break;
                }
            }
            if (!found_garage) {
                printf("Garage not found.\n");
            }
        }
        
        else if (strcmp(commands, "UTILIZATION_REPORT") == 0) {
            generateGarageUtilizationReport(&campus);
            
        }
        else if (strcmp(commands, "RESIZE") == 0) {
            scanf("%s %d", garage_name, &capacity);
            int resized = 0;
            for (int k = 0; k < num_garages; k++) 
            {
                if (strcmp(campus.garages[k]->garage_name, garage_name) == 0) 
                {
                    resized = resizeGarage(campus.garages[k], capacity);
                    break;
                }
            }
        }
        
        else if (strcmp(commands, "SEARCH_OWNER") == 0) 
        {
            scanf("%s", owner);
            displayVehiclesByOwner(&campus, owner);
        }
        else if (strcmp(commands, "RELOCATE") == 0) 
        {
            scanf("%s %s", license, garage_name);
            relocateVehicle(&campus, license, garage_name);
        }
        
        else if (strcmp(commands, "COUNT_TOTAL") == 0) 
        {
            int total_vehicles = countTotalVehicles(&campus);
            printf("%d\n", total_vehicles);
        }
        
        else if (strcmp(commands, "REGISTER_VEHICLE") == 0) 
        {
            scanf("%s %s", license, owner);
            RegisteredVehicle** temp = realloc(campus.registered_vehicles, sizeof(RegisteredVehicle*) * (campus.total_registered_vehicles + 1));
            if (temp == NULL) 
            {
                printf("Memory allocation failed.\n");
                exit(1);
            }
            campus.registered_vehicles = temp;
            campus.registered_vehicles[campus.total_registered_vehicles++] = createRegisteredVehicle(license, owner);
            printf("REGISTERED\n");
        }
        
        else if (strcmp(commands, "REMOVE_VEHICLE_GARAGE") == 0) 
        {
            scanf("%s", license);
            int found_vehicle = 0;
            for (int k = 0; k < num_garages; k++) 
            {
                if (removeVehicleFromGarage(campus.garages[k], license)) 
                {
                    found_vehicle = 1;
                    printf("REMOVED FROM %s\n", campus.garages[k]->garage_name);
                    break;
                }
            }
            if (!found_vehicle) 
            {
                printf("NOT FOUND IN CAMPUS\n");
            }
        }
        else if (strcmp(commands, "REMOVE_GARAGE") == 0) 
        {
            scanf("%s", garage_name);
            removeGarage(&campus, garage_name);
        }
    }

    // Free all allocated memory
    free_memory(&campus);
    return 0;
}
