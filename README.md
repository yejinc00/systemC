# SystemC

### SimpleBus
------------
- 2 master, 2 slave
- Address bandwidth = 8bits
- Data bandwidth = 32bits
- Slave 0's address : 0x00~0x1F
- Slave 1's address : 0x20~0x3F
- FSM
<img width="500" alt="SimpleBusFSM" src="https://user-images.githubusercontent.com/57093610/94173213-8ec1b900-fece-11ea-9e49-08098132556e.PNG">

--------------
### Master


### Slave


### Tips
- 다음 코드의 정체는?
```bash
sc_report_handler::set_actions("/IEEE_Std_1666/deprecated", SC_DO_NOTHING);
```
