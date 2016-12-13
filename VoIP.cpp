/*
 * Copyright (c) 2010-2016 Stephane Poirier
 *
 * stephane.poirier@oifii.org
 *
 * Stephane Poirier
 * 3532 rue Ste-Famille, #3
 * Montreal, QC, H2X 2L1
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <iomanip>
#include <iostream>
#include <cstdlib>


////////////////////////////////////////////////////////////
// Function prototypes
// (I'm too lazy to put them into separate headers...)
////////////////////////////////////////////////////////////
void DoClient(unsigned short Port);
void DoServer(unsigned short Port);


////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
int main()
{
    // Choose a random port for opening sockets (ports < 1024 are reserved)
    const unsigned short Port = 2435;

    // Client or server ?
    char Who;
    std::cout << "Do you want to be a server ('s') or a client ('c') ? ";
    std::cin  >> Who;

    if (Who == 's')
    {
        // Run as a server
        DoServer(Port);
    }
    else
    {
        // Run as a client
        DoClient(Port);
    }

    // Wait until the user presses 'enter' key
    std::cout << "Press enter to exit..." << std::endl;
    std::cin.ignore(10000, '\n');

    return EXIT_SUCCESS;
}
