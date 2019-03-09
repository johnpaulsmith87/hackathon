import { Injectable } from '@angular/core';
import { HttpClient, HttpHeaders } from '@angular/common/http';
import { Observable } from 'rxjs';

@Injectable({
  providedIn: 'root'
})
export class DiagnosticsService {

  private botUrl = "";

  constructor(private http: HttpClient) { }

  getTurningAngle(): Observable<Number> {
    return this.http.get<number>(`${this.botUrl}/turningAngle`);
  }
  getVelocity(): Observable<Number> {
    return this.http.get<number>(`${this.botUrl}/velocity`);
  }
  getUltrasonic(): Observable<Number> {
    return this.http.get<number>(`${this.botUrl}/ultrasonic`);
  }
}
