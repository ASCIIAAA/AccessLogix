#include "UserDB.h"
#include <avr/pgmspace.h>

// --- STORE DATA IN FLASH MEMORY (Saves ~140 Bytes of RAM) ---
const char u1_uid[] PROGMEM = "570064A276"; const char u1_name[] PROGMEM = "Ex-Student";   const char u1_role[] PROGMEM = "S"; const char u1_pass[] PROGMEM = "";
const char u2_uid[] PROGMEM = "570066B113"; const char u2_name[] PROGMEM = "Ananya";       const char u2_role[] PROGMEM = "A"; const char u2_pass[] PROGMEM = "A57845";
const char u3_uid[] PROGMEM = "570066AF3E"; const char u3_name[] PROGMEM = "Prof. Sonali"; const char u3_role[] PROGMEM = "P"; const char u3_pass[] PROGMEM = "B6501";
const char u4_uid[] PROGMEM = "570067BA4D"; const char u4_name[] PROGMEM = "Aditi";        const char u4_role[] PROGMEM = "S"; const char u4_pass[] PROGMEM = "D9877";
const char u5_uid[] PROGMEM = "5700657DA1"; const char u5_name[] PROGMEM = "Anushka";      const char u5_role[] PROGMEM = "S"; const char u5_pass[] PROGMEM = "D8709";

// Array of pointers to the Flash data
const char* const all_uids[] PROGMEM = {u1_uid, u2_uid, u3_uid, u4_uid, u5_uid};
const char* const all_names[] PROGMEM = {u1_name, u2_name, u3_name, u4_name, u5_name};
const char* const all_roles[] PROGMEM = {u1_role, u2_role, u3_role, u4_role, u5_role};
const char* const all_pass[] PROGMEM = {u1_pass, u2_pass, u3_pass, u4_pass, u5_pass};

UserDB::UserDB() {}

void UserDB::init() {
    // Nothing to do, data is in Flash
}

User* UserDB::findUser(const char* scannedUid) {
    char flashUidBuf[12];
    
    for(int i = 0; i < 5; i++) {
        // Read UID from Flash to temp buffer
        strcpy_P(flashUidBuf, (char*)pgm_read_word(&(all_uids[i])));
        
        if(strcasecmp(scannedUid, flashUidBuf) == 0) {
            // MATCH FOUND! Load data from Flash into the single tempUser object
            char t_name[15];
            char t_role[3];
            char t_pass[10];
            
            strcpy_P(t_name, (char*)pgm_read_word(&(all_names[i])));
            strcpy_P(t_role, (char*)pgm_read_word(&(all_roles[i])));
            strcpy_P(t_pass, (char*)pgm_read_word(&(all_pass[i])));
            
            // Re-use the existing "setFromFlash" logic but manually
            // Actually, just use the constructor since we pulled data to stack
            tempUser = User(flashUidBuf, t_name, t_role, t_pass, true); // Assuming active=true for simplicity
            
            // Hardcode the inactive ex-student logic if needed
            if (i == 0) tempUser = User(flashUidBuf, t_name, t_role, t_pass, false);
            
            return &tempUser;
        }
    }
    return nullptr; 
}