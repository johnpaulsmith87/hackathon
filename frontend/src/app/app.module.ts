import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';

import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { JoystickComponent } from './joystick/joystick.component';
import { DiagnosticsComponent } from './diagnostics/diagnostics.component';

import { SocketIoModule, SocketIoConfig} from 'ngx-socket-io'

//192.168.43.196:8080
const config: SocketIoConfig = {url: 'http://192.168.43.196:8080', options: {}}
@NgModule({
  declarations: [
    AppComponent,
    JoystickComponent,
    DiagnosticsComponent
  ],
  imports: [
    BrowserModule,
    AppRoutingModule,
    SocketIoModule.forRoot(config)
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }
