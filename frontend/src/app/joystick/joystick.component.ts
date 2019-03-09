import { Component, OnInit } from '@angular/core';
import { ControlService } from '../control.service'

@Component({
  selector: 'app-joystick',
  templateUrl: './joystick.component.html',
  styleUrls: ['./joystick.component.css']
})
export class JoystickComponent implements OnInit {

  constructor(private controlService: ControlService) { }

  ngOnInit() {
  }

  stop() {
  
  }

  forward() {

  }

  backward() {

  }

  left() {

  }

  right() {

  }

}
