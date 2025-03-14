1. How does the remote client determine when a command's output is fully received from the server, and what techniques can be used to handle partial reads or ensure complete message transmission?
The remote client determines that a command's output is fully received when it detects the EOF character at the end of the received data. Some techniques that can be used include buffering, looping recv() or using delimiters

2. This week's lecture on TCP explains that it is a reliable stream protocol rather than a message-oriented one. Since TCP does not preserve message boundaries, how should a networked shell protocol define and detect the beginning and end of a command sent over a TCP connection? What challenges arise if this is not handled correctly?
A networked shell protocol should define clear message boundaries by: Using explicit end markers, tracking message lengths and ensuring complete reads

Some challenges that may come up are message fragmentation, message concatenation and race conditions where if different parts of a command are read at different times, some execution errors may occur

3. Describe the general differences between stateful and stateless protocols.
Stateful protocols maintain session information between client and server interactions while stateless protocols treat each request independently, without memory of previous interactions.

4. Our lecture this week stated that UDP is "unreliable". If that is the case, why would we ever use it?
UDP is useful in scenarios where speed is the priority over reliability such as in real time applications.

5. What interface/abstraction is provided by the operating system to enable applications to use network communications?
The Berkeley Sockets API