import { ref, get, update } from 'firebase/database';
import { database } from '../../firebaseConfig'; // Make sure the path is correct

// This function determines the next system identifier based on the existing systems
const getNextSystemIdentifier = (systems) => {
    if (!systems.length) return 'SystemA';

    const letterSystems = systems.filter(s => s.startsWith('System') && isNaN(s.slice(6)));
    letterSystems.sort();

    let nextIdentifier = '';

    if (letterSystems.length) {
        const lastLetterIdentifier = letterSystems[letterSystems.length - 1].slice(6); // Get the last letter part
        const lastCharCode = lastLetterIdentifier.charCodeAt(0);
        if (lastCharCode < 90) { // ASCII code of 'Z' is 90
            nextIdentifier = `System${String.fromCharCode(lastCharCode + 1)}`;
        } else {
            // If last letter is Z, then start numbering or increment the highest number
            const numberSystems = systems.filter(s => s.startsWith('System') && !isNaN(s.slice(6)));
            if (numberSystems.length) {
                const highestNumber = Math.max(...numberSystems.map(s => parseInt(s.slice(6))));
                nextIdentifier = `System${highestNumber + 1}`;
            } else {
                nextIdentifier = 'System1';
            }
        }
    } else {
        // If there are no letter-based systems, determine if there are any number-based systems
        const numberSystems = systems.filter(s => s.startsWith('System') && !isNaN(s.slice(6)));
        if (numberSystems.length) {
            const highestNumber = Math.max(...numberSystems.map(s => parseInt(s.slice(6))));
            nextIdentifier = `System${highestNumber + 1}`;
        } else {
            // If there are no systems, start with A
            nextIdentifier = 'SystemA';
        }
    }

    return nextIdentifier;
};


// This function adds a new system to the user's account in the database
export const addNewSystem = async (userId) => {
    // Reference to the systems of the user
    const userSystemsRef = ref(database, `Registered Users/${userId}`);

    try {
        // Fetch the current systems
        const snapshot = await get(userSystemsRef);
        let systems = [];
        if (snapshot.exists()) {
            systems = Object.keys(snapshot.val());
        }

        // Determine the next system identifier
        const nextSystemIdentifier = getNextSystemIdentifier(systems);

        // Updates to be made in the database
        const updates = {};
        updates[nextSystemIdentifier] = {
            Location: "Canada", // Initial data for the new system
            // ... any other initial data
        };

        // Update the database with the new system
        await update(userSystemsRef, updates);
        console.log(`New system ${nextSystemIdentifier} added to user ${userId} with location Canada.`);
        return nextSystemIdentifier; // Return the new system's identifier for further processing
    } catch (error) {
        console.error("Error adding new system:", error);
        throw error; // Rethrow the error after logging it
    }
};
