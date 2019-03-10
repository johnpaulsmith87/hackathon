import { Injectable } from '@angular/core';
import { Socket } from 'ngx-socket-io';
import { Observable } from 'rxjs';

@Injectable({
  providedIn: 'root'
})
export class SerialService {

  data: Observable<any> = this.socket.fromEvent<any>('data')
  status: Observable<any> = this.socket.fromEvent<any>('status')

  constructor(private socket: Socket) { }
}
