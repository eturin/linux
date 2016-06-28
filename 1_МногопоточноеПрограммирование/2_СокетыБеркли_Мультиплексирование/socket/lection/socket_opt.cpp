int optval = 1;
setsockopt(s,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(optval)); //для повторного использования сокета после завершения и повторного подхватывания (иначе будет висеть некоторое время)

timeval tv;
tv.tv_sec=16; //16 секунд на таймаут отправки и приема 
tv.tv_usec=0; //милисекунды
//SO_RCVTIMEO -- прием
//SO_SNDTIMEO -- отправка
setsockopt(s,SOL_SOCKET,SO_RCVTIMEO,(char*)&tv,sizeof(tv));
