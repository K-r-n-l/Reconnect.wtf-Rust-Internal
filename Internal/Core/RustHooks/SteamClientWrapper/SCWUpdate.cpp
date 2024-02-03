#include "SCWUpdate.h"
#include "../../Esp/Visuals.h"


ULONG64 SteamID3To64(ULONG64 id)
{
	return ((1ULL << 56) | (1ULL << 52) | (1ULL << 32) | id);
}

std::string base64_encode(const std::string& input) {
    const std::string base64_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";

    std::string encoded;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];

    for (const auto& c : input) {
        char_array_3[i++] = c;

        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xFC) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xF0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0F) << 2) + ((char_array_3[2] & 0xC0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3F;

            for (i = 0; i < 4; i++) {
                encoded += base64_chars[char_array_4[i]];
            }
            i = 0;
        }
    }

    if (i != 0) {
        for (j = i; j < 3; j++) {
            char_array_3[j] = '\0';
        }

        char_array_4[0] = (char_array_3[0] & 0xFC) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xF0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0F) << 2) + ((char_array_3[2] & 0xC0) >> 6);

        for (j = 0; j < i + 1; j++) {
            encoded += base64_chars[char_array_4[j]];
        }

        while (i++ < 3) {
            encoded += '=';
        }
    }

    return encoded;
}
uintptr_t hexStringToUintptr(const std::string& str) {
    uintptr_t result = 0;

    // Skip leading zeros
    size_t startIndex = 0;
    while (startIndex < str.size() && str[startIndex] == '0')
        ++startIndex;

    for (size_t i = startIndex; i < str.size(); ++i) {
        char c = str[i];
        uintptr_t value;

        if (c >= '0' && c <= '9') {
            value = c - '0';
        }
        else if (c >= 'a' && c <= 'f') {
            value = c - 'a' + 10;
        }
        else if (c >= 'A' && c <= 'F') {
            value = c - 'A' + 10;
        }
        else {
            // Invalid character in the string, handle error or return a default value.
            // For simplicity, we'll just return 0 in this case.
            return 0;
        }

        result = (result << 4) | value;
    }

    return result;
}

int isspace(int c) {
    // Standard white-space characters: space, tab, carriage return, line feed, vertical tab, and form feed
    return (c == ' ' || c == '\t' || c == '\r' || c == '\n' || c == '\v' || c == '\f');
}


unsigned long strtoul(const char* str, char** endptr, int base) {
    unsigned long result = 0;
    int sign = 1;

    // Skip leading whitespace
    while (isspace(*str)) {
        str++;
    }

    // Check for optional sign
    if (*str == '+' || *str == '-') {
        sign = (*str == '-') ? -1 : 1;
        str++;
    }

    // Check for the base if not provided explicitly
    if (base == 0) {
        if (*str == '0') {
            str++;
            if (*str == 'x' || *str == 'X') {
                base = 16;
                str++;
            }
            else {
                base = 8;
            }
        }
        else {
            base = 10;
        }
    }

    // Convert the string to an unsigned long integer
    while (*str) {
        int digit;
        if (*str >= '0' && *str <= '9') {
            digit = *str - '0';
        }
        else if (*str >= 'A' && *str <= 'Z') {
            digit = *str - 'A' + 10;
        }
        else if (*str >= 'a' && *str <= 'z') {
            digit = *str - 'a' + 10;
        }
        else {
            break; // Invalid character
        }

        if (digit >= base) {
            break; // Invalid digit for the given base
        }

        result = result * base + digit;
        str++;
    }

    // Set the end pointer if provided
    if (endptr) {
        *endptr = (char*)str;
    }

    return result * sign;
}

unsigned long stoul_crt(const char* str, size_t* idx, int base) {
    const char* begin = str;
    char* end = nullptr;
    auto errnoz = 0;

    // Use strtol to parse the unsigned long value
    unsigned long value = strtoul(begin, &end, base);

    // Check for errors during conversion
    if (errnoz == ERANGE) {
        // Handle range error (value out of range for unsigned long)
        // Set value to ULONG_MAX or 0 depending on the sign of the value
        value = (value < 0) ? ULONG_MAX : 0;
    }
    else if (end == begin) {
        // Handle case where no conversion was performed
        // Set value to 0 and end to str.c_str() to indicate no characters were parsed
        value = 0;
        end = (char*)begin;
    }
    else if (idx != nullptr) {
        // Calculate the index of the first character after the parsed value
        *idx = static_cast<size_t>(end - begin);
    }

    return value;
}
//
////
void Hooks::SCRUpdate(AssemblyCSharp::SteamClientWrapper* _This)
{
	static bool injected_once = false;


	if (!injected_once)
	{
		static uintptr_t WebClientClass = 0; if (!WebClientClass) WebClientClass = (uintptr_t)CIl2Cpp::FindClass(SECURE("System.Net"), SECURE("WebClient"));

		if (System_Net::WebClient* webclient = reinterpret_cast<System_Net::WebClient*>(CIl2Cpp::il2cpp_object_new((void*)WebClientClass)))
		{

            webclient->_cctor();


             const auto type = std::string(SECURE("retrieve_address"));
             auto steamid = Steamworks::SteamClient::get_SteamId().Value;
             auto steamidfix = SteamID3To64(steamid);
             auto steamUsername = _This->GetSteamUserName(steamidfix);
             auto steamUsernameStr = base64_encode(steamUsername->string_safe());
             auto str = base64_encode(std::to_string(steamidfix));

             auto request_msg = std::wstring(SECURE(L"https://xcheats.dev/rustapi/retrieve_offsets.php?type=")).append(type.begin(), type.end());
             auto request_msg_str = std::string(request_msg.begin(), request_msg.end());

             auto resp = webclient->DownloadString(request_msg_str.c_str());

             if (!GetListAddress)
             {
                 GetListAddress = static_cast<DWORD>(stoul_crt(resp->string_safe().c_str(), nullptr, 10));
             }

             auto gameoffset = memory::Read<uintptr_t>(m_game_assembly + GetListAddress);
             if (!memory::IsAddressValid(gameoffset))
             {
                 LI_FIND(MessageBoxA)(0, SECURE("An offset is outdated (1), Please make a ticket."), SECURE("Game Update"), MB_ICONASTERISK);
             }


             const auto type2 = std::string(SECURE("retrieve_method"));
       
             auto request_msg2 = std::wstring(SECURE(L"https://xcheats.dev/rustapi/retrieve_offsets.php?type=")).append(type2.begin(), type2.end());
             auto request_msg_str2 = std::string(request_msg2.begin(), request_msg2.end());

             auto resp2 = webclient->DownloadString(request_msg_str2.c_str());

             if (!GetListMethodAddress)
             {
                 GetListMethodAddress = static_cast<DWORD>(stoul_crt(resp2->string_safe().c_str(), nullptr, 10));
             }


            //const auto type = std::string(SECURE("request_config"));
            //auto steamid = Steamworks::SteamClient::get_SteamId().Value;
            //auto steamidfix = SteamID3To64(steamid);
            //auto steamUsername = _This->GetSteamUserName(steamidfix);
            //auto steamUsernameStr = base64_encode(steamUsername->string_safe());
            //auto str = base64_encode(std::to_string(steamidfix));

            //auto request_msg = std::wstring(SECURE(L"https://xcheats.dev/rustapi/index.php?type=")).append(type.begin(), type.end()).append(SECURE(L"&username=")).append(steamUsernameStr.begin(), steamUsernameStr.end()).append(SECURE(L"&steamID=")).append(str.begin(), str.end());
            //auto request_msg_str = std::string(request_msg.begin(), request_msg.end());


            //const auto uri = webclient->GetUri(request_msg_str.c_str());
            //webclient->DownloadStringAsync(uri);


           // const auto type = std::string(SECURE("player_request"));
           // auto steamid = Steamworks::SteamClient::get_SteamId().Value;
           // auto steamidfix = SteamID3To64(steamid);
           // auto steamUsername = _This->GetSteamUserName(steamidfix);
           // auto steamUsernameStr = base64_encode(steamUsername->string_safe());
           // auto str = base64_encode(std::to_string(steamidfix));

           // auto request_msg = std::wstring(SECURE(L"https://xcheats.dev/rustapi/index.php?type=")).append(type.begin(), type.end()).append(SECURE(L"&username=")).append(steamUsernameStr.begin(), steamUsernameStr.end()).append(SECURE(L"&steamID=")).append(str.begin(), str.end());
           // auto request_msg_str = std::string(request_msg.begin(), request_msg.end());

           //auto resp = webclient->DownloadString(request_msg_str.c_str());

           //
           //printfz("\n resp- %ls", resp->c_str());

			/*webclient->_cctor();

			const auto type = std::string(SECURE("authenticate"));
			auto steamid = Steamworks::SteamClient::get_SteamId().Value;
			auto steamidfix = SteamID3To64(steamid);
			auto steamUsername = _This->GetSteamUserName(steamidfix);
			auto steamUsernameStr = base64_encode(steamUsername->string_safe());
			auto str = base64_encode(std::to_string(steamidfix));

			auto request_msg = std::wstring(SECURE(L"https://xcheats.dev/rustapi/index.php?type=")).append(type.begin(), type.end()).append(SECURE(L"&username=")).append(steamUsernameStr.begin(), steamUsernameStr.end()).append(SECURE(L"&steamID=")).append(str.begin(), str.end());
			auto request_msg_str = std::string(request_msg.begin(), request_msg.end());

			webclient->DownloadString(request_msg_str.c_str());*/

		}

		injected_once = true;
	}

	return Hooks::SCRUpdatehk.get_original< decltype(&SCRUpdate)>()(_This);
}