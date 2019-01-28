import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';
import { NgbModule } from '@ng-bootstrap/ng-bootstrap';
import { HttpClientModule } from '@angular/common/http';

import { AppComponent } from './app.component';
import { ConfigService } from './services/config.service';
import { PrettySizeModule, PrettySizeOptions } from 'angular-pretty-size';

@NgModule({
  declarations: [
    AppComponent
  ],
  imports: [
    NgbModule,
    BrowserModule,
    HttpClientModule,
    PrettySizeModule.forRoot({
      provide: PrettySizeOptions,
      useValue: {
          units: [' Byte', ' KiB', ' MiB', ' GiB', ' TiB', ' PiB', ' EiB', ' ZiB', ' YiB'],
          divisor: 1024,
          scale: 2, // 0, 1 or 2
      },
  }),
  ],
  providers: [ConfigService],
  bootstrap: [AppComponent]
})
export class AppModule { }
