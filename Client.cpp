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
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <iostream>


const sf::Uint8 AudioData   = 1;
const sf::Uint8 EndOfStream = 2;


////////////////////////////////////////////////////////////
/// Specialization of audio recorder for sending recorded audio
/// data through the network
////////////////////////////////////////////////////////////
class NetworkRecorder : public sf::SoundRecorder
{
public :

    ////////////////////////////////////////////////////////////
    /// Constructor
    ///
    /// \param Socket : Socket that holds the connection with the server
    ///
    ////////////////////////////////////////////////////////////
    NetworkRecorder(sf::SocketTCP Socket) :
    mySocket(Socket)
    {

    }

private :

    ////////////////////////////////////////////////////////////
    /// /see SoundRecorder::ProcessSamples
    ///
    ////////////////////////////////////////////////////////////
    virtual bool OnProcessSamples(const sf::Int16* Samples, std::size_t SamplesCount)
    {
        // Pack the audio samples into a network packet
        sf::Packet PacketOut;
        PacketOut << AudioData;
        PacketOut.Append(Samples, SamplesCount * sizeof(sf::Int16));

        // Send the audio packet to the server
        return mySocket.Send(PacketOut) == sf::Socket::Done;
    }

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    sf::SocketTCP mySocket; ///< Socket used to communicate with the server
};


////////////////////////////////////////////////////////////
/// Create a client, connect it to a running server and
/// start sending him audio data
///
////////////////////////////////////////////////////////////
void DoClient(unsigned short Port)
{
    // Check that the device can capture audio
    if (sf::SoundRecorder::CanCapture() == false)
    {
        std::cout << "Sorry, audio capture is not supported by your system" << std::endl;
        return;
    }

    // Ask for server address
    sf::IPAddress ServerAddress;
    do
    {
        std::cout << "Type address or name of the server to connect to : ";
        std::cin  >> ServerAddress;
    }
    while (!ServerAddress.IsValid());

    // Create a TCP socket for communicating with server
    sf::SocketTCP Socket;

    // Connect to the specified server
    if (Socket.Connect(Port, ServerAddress) != sf::Socket::Done)
        return;
    std::cout << "Connected to server " << ServerAddress << std::endl;

    // Wait for user input...
    std::cin.ignore(10000, '\n');
    std::cout << "Press enter to start recording audio";
    std::cin.ignore(10000, '\n');

    // Create a instance of our custom recorder
    NetworkRecorder Recorder(Socket);

    // Start capturing audio data
    Recorder.Start(44100);
    std::cout << "Recording... press enter to stop";
    std::cin.ignore(10000, '\n');
    Recorder.Stop();

    // Send a "end-of-stream" packet
    sf::Packet PacketOut;
    PacketOut << EndOfStream;
    Socket.Send(PacketOut);

    // Close the socket when we're done
    Socket.Close();
}
